#!/bin/python3
# -*- coding: utf-8 -*-

from jinja2 import  Environment, FileSystemLoader

# Load given filename
def load_charset( filename ):
    return  open( filename, "r", encoding="utf-8").read().replace( '\n', '' )

# load pl charset into variable
# char position is directly translated into byte value
# first char is 0x00, second is 0x01, third 0x02
# '\n' - newline characters are stripped
charset = load_charset( "templates/charset_pl.txt" )

# convert ascii text into byte array
def encode_charset( text ):
    text = list( text )

    for idx, c in enumerate( text ):
        if c in charset:
            text[ idx ] = '0x%02x' % charset.index( c )
    return ", ".join( text )

# convert ascii text into byte array + $80 ( underline charset )
def underline_text( text, mask ):
    text = list( text )

    for idx, c in enumerate( text ):
        if c in charset:
            char = charset.index( c )
            if mask[ idx ] == '-':
                char += 128
            text[ idx ] = '0x%02x' % char
    return ", ".join( text )

# insert number or arrow from $80 charset before string
def menu_opt( option ):
    if option == '<':
        option = '0x1f, '
    else:
        option = '0x%02x, ' % ( charset.index( option ) + 0x40 )
    return option

# generate indexes for the 'c' language common.h file and commont text arrays
def generate_common_h_index_array_jinja2( config ):
    arrays_info = {}
    enum_labels = []
    text_variables = []
    if config.get( 'Txt' ):
        arrays_info = {
                'pragma_label': 'Txt',
                'array_label' : config[ 'Txt' ].get( 'array_label' )
        }
        if config[ 'Txt' ].get( 'contents' ):
            # if there is 'Txt' and 'contents' in 'config'
            for p in config[ 'Txt' ][ 'contents' ]:
                prefix = 'TXT' if not p.get( 'prefix' ) else p[ 'prefix' ]
                enum_labels.append({'prefix': prefix, 'label': p['id']})
                # if there is a 'common' in config database then create variable in common.h file
                # it will be common for all language files ( texts*.c )
                if p.get( 'common' ):
                    text = p[ 'common' ]
                    if p.get( 'common_m' ):     # if mask exists apply it to text
                        # print("common mask variable :  ", mask)
                        text = underline_text( text, p[ mask ] )
                    else:
                        text = encode_charset( text )
                    if p.get( 'menu_opt' ):
                        text = menu_opt( p.get( 'menu_opt' ) ) + text
                    text_variables.append({'prefix': prefix, 'label': p['id'], 'text': p['common'], 'bytearray': text})
    return {**arrays_info, 'enum_labels': enum_labels, 'text_variables': text_variables}

# generate index arrays for 'common.h' file
def generate_common_h_file_index_arrays_jinja2( config ):
    index_arrays = []
    for k, v in config.items():
        section = {'pragma_label': k, 'array_label': v.get('array_label'), 'indexes_count': len(v.get("contents"))}
        contents = []
        for p in v.get( "contents" ):
            prefix = "TXT" if not p.get("prefix") else p["prefix"]
            contents.append('%s_IDX_%s' % (prefix, p['id']))
        index_arrays.append({**section, 'contents': contents})
    return {'index_arrays': index_arrays }

# generate index array for given 'lang'
def generate_c_file_index_arrays_jinja2( config, lang ):
    index_arrays = []
    for k, v in config.items():
        section = {'pragma_label': k, 'array_label': v.get('array_label'), 'indexes_count': len(v.get("contents"))}
        contents = []
        for p in v.get( "contents" ):
            prefix = "TXT" if not p.get("prefix") else p["prefix"]
            if p.get( "common" ):
                contents.append('%s_%s' % (prefix, p['id']))
            else:
                contents.append('%s_%s_%s' % (prefix, lang.upper(), p['id']))
        index_arrays.append({**section, 'contents': contents})
    return {'index_arrays': index_arrays, 'lang': lang}

# generate text arrays for texts*.c file, with given 'lang' and apply text_filter
def generate_c_file_text_arrays_jinja2( config, lang, text_filter ):
    text_arrays = []
    for k, v in config.items():
        section = {'pragma_label': k, 'array_label': v.get('array_label'), 'indexes_count': len(v.get("contents"))}
        array_length = 0
        contents = []
        for p in v.get( "contents" ):
            if p.get( "common" ):
                # "common" definition is already in common.h file
                # so we will get index from common.h file
                # in index array part
                pass
            else:
                text = p[ lang ]

                # make human readable comment from text
                if isinstance( text, list ):
                    comment = '"\n//           "'.join(text)
                else:
                    comment = f'{text}'

                # merge text into one single string if text is a 'list' type
                if isinstance(text, list):
                    text = "".join( text )

                # convert text into bytearray, with mask (+0x80) or without (+0x00)
                mask = "%s_m" % lang
                if p.get( mask ):
                    text = underline_text( text, p[ mask ] )    # text + 0x80
                else:
                    text = text_filter( text )                  # text + 0x00

                # add appropriate menu option symbol if this is menu option
                if p.get( "menu_opt" ):
                    text = menu_opt( p.get( "menu_opt" ) ) + text

                # create and add label data, prefix and name as dictionary
                prefix = "TXT" if not p.get("prefix") else p["prefix"]
                label = {'prefix': prefix, 'name': p['id'], 'bytearray': text, 'comment': comment}
                contents.append(label)

                # count text array size in bytes ( add 1 for zero terminated string )
                # currenty 0x00 byte is added in templates and not here
                # this can be misleading and create problems
                array_length += text.count('x') + 1

        text_arrays.append({**section, 'contents': contents, 'array_length': array_length})
    return {'text_arrays': text_arrays, 'lang': lang}

def write( filename, content ):
    with open(filename, 'w', encoding="utf-8") as fh:
        fh.write( content )

def create_lang_files_jinja2( config, lang, dot_c_template, dot_h_template, dst_filename ):
    
    environment  = Environment(loader=FileSystemLoader("templates/"))

    # create dot_c_file
    text_arrays  = generate_c_file_text_arrays_jinja2( config, lang, encode_charset )
    index_arrays = generate_c_file_index_arrays_jinja2( config, lang )
    template     = environment.get_template( dot_c_template )
    write( dst_filename + '.c', template.render( {**text_arrays, **index_arrays} ) )
    
    # create dot_h_file
    template     = environment.get_template( dot_h_template )
    write( dst_filename + '.h', template.render( {**text_arrays, 'lang': lang} ) )

# create 'common.h' file baseing on "config" data structure
def create_common_h_file_jinja2( config, dot_h_template, dst_filename ):

    environment      = Environment(loader=FileSystemLoader("templates/"))

    # create common.h file
    common_h_content = generate_common_h_index_array_jinja2( config )
    index_arrays     = generate_common_h_file_index_arrays_jinja2( config )
    template         = environment.get_template( dot_h_template )
    write( dst_filename, template.render( {**common_h_content, **index_arrays} ) )












