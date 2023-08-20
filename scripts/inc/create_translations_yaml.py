#!/bin/python3
# -*- coding: utf-8 -*-

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
def underline_text_pl( text, mask ):
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

# generate indexes for the 'c' language common.h file
def generate_common_h_index_array( config ):
    out = []
    common = []
    if config.get( 'main_contents' ):
        common.append( '//     "pragma_label": %s,' % (config[ 'main_contents' ].get( 'pragma_label' ) ) )
        common.append( '//     "array_label": %s,\n' % (config[ 'main_contents' ].get( 'array_label' ) ) )
        if config[ 'main_contents' ].get( 'contents' ):
            # if there is 'main_contents' and 'contents' in 'data.json' file
            for p in config[ 'main_contents' ][ 'contents' ]:
                prefix = 'TXT' if not p.get( 'prefix' ) else p[ 'prefix' ]
                out.append( '    %s_IDX_%s,' % ( prefix, p[ 'id' ] )  )
                # if there is a 'common' in *.json file entry then create variable in common.h file
                # it will be common for all language files ( texts*.c )
                if p.get( 'common' ):
                    p1 = 'static const char %s_%s[] =' % ( prefix, p[ 'id' ] )
                    text = p[ 'common' ]
                    common.append( f'//     "common": "{text}"' )
                    if p.get( 'common_m' ):     # common text mask label
                        text = underline_text_pl( text, p[ mask ] )
                    else:
                        text = encode_charset( p[ 'common' ] )
                    if p.get( 'menu_opt' ):
                        text = menu_opt( p.get( "menu_opt" ) ) + text
                    common.append( '%s {%s, 0x00};' % (p1, text))
    out[ -1 ] = out[ -1 ].strip(',')            # delete comma in last array element
    out.append( '};\n' )
    out.extend( common )
    out.append( '' )
    return out

# generate index array for given 'lang'
# def generate_texts_c_generic_lang_file_index_arrays( config, lang ):
def generate_c_file_index_arrays( config, lang ):
    out = []
    for k, v in config.items():
        append_after_comments = []
        if v.get( "pragma_label" ):
            out.append( '\n//     "pragma_label": %s,' % (v.get( "pragma_label" ) ) )
            append_after_comments.append( "\n#pragma data ( txt%s%s )" % ( lang.capitalize(), v.get( "pragma_label" ) ) )
        if v.get( "array_label" ):
            out.append( '//     "array_label": %s,' % (v.get( "array_label" ) ) )
            append_after_comments.append( "__export const char* %s_%s[] = {" % ( v.get( "array_label" ), lang.upper() ) )
        out.extend( append_after_comments )
        for p in v.get( "contents" ):
            prefix = "TXT" if not p.get("prefix") else p["prefix"]
            if p.get( "common" ):
                out.append( '    %s_%s,' % ( prefix, p['id'] )  )
            else:
                out.append( '    %s_%s_%s,' % ( prefix, lang.upper(), p['id'] )  )
        out[ -1 ] = out[ -1 ].strip(',')            # delete comma in last array element
        out.append( "};" )
    return out

# generate index array for 'pl' language
# def generate_texts_c_pl_file_index_arrays( config, lang ):
def generate_c_file_pl_index_arrays( config, lang ):
    out = []
    for k, v in config.items():
        append_after_comments = []
        if v.get( "pragma_label" ):
            out.append( '\n//     "pragma_label": %s,' % (v.get( "pragma_label" ) ) )
            append_after_comments.append( "\n#pragma data ( txt%s%s )" % ( lang.capitalize(), v.get( "pragma_label" ) ) )
        if v.get( "array_label" ):
            out.append( '//     "array_label": %s,' % (v.get( "array_label" ) ) )
            append_after_comments.append( "__export const char* %s[] = {" % v.get( "array_label" ) )
        out.extend( append_after_comments )
        for p in v.get( "contents" ):
            prefix = "TXT" if not p.get("prefix") else p["prefix"]
            if p.get( "common" ):
                out.append( '    %s_%s,' % ( prefix, p['id'] )  )
            else:
                out.append( '    %s_%s_%s,' % ( prefix, lang.upper(), p['id'] )  )
        out[ -1 ] = out[ -1 ].strip(',')            # delete comma in last array element
        out.append( "};" )
    return out

# generate text arrays for texts*.c file, with given 'lang' and apply text_filter
def generate_c_file_text_arrays( config, lang, text_filter ):
    out = []
    for k, v in config.items():
        out.append( '\n//     "pragma_label": "%s"' % ( v.get('pragma_label') ) )
        out.append( '//     "array_label" : "%s"\n' % ( v.get('array_label') ) )
        for p in v.get( "contents" ):
            prefix = "TXT" if not p.get("prefix") else p["prefix"]
            if p.get( "common" ):
                # "common" definition is already in common.h file
                # so we will get index from common.h file
                # in index array part
                pass
            else:
                p1 = 'const char %s_%s_%s[] =' %( prefix, lang.upper(), p['id'] )
                text = p[ lang ]
                if isinstance( text, list ):
                    out.append( f'//     "{lang}": "' + '"\n//           "'.join(text) + '"' )
                else:
                    out.append( f'//     "{lang}": "{text}"' )
                # support for json multiline values
                if isinstance(text, list):
                    text = "".join( text )
                mask = "%s_m" % lang
                if p.get( mask ):
                    text = underline_text_pl( text, p[ mask ] )
                else:
                    text = text_filter( text )
                if p.get( "menu_opt" ):
                    text = menu_opt( p.get( "menu_opt" ) ) + text
                out.append( "%s {%s, 0x00};" % ( p1, text ) )
    return out

# write common.h file in templates/ directory
def write_common_h_file( fullpath, template_fullpath, index_array ):
    fh = open(fullpath, "w", encoding="utf-8")
    template = open( template_fullpath, "r" ).read()
    fh.write( template.replace( "{{ content }}", "\n".join( index_array ) ) )
    fh.close()

# write texts{{ LANGUAGE }}.c file with given template, text arrays, index array and language
def write_texts_c_file( dst_filename, src_template, text_array, index_array, lang ):
    out = []
    out.append( "#pragma data ( txt%sTxtData )\n" % lang.capitalize() )
    out.append( "\n".join( text_array ) )
    out.append( "\n".join( index_array ) )

    fh = open(dst_filename, "w", encoding="utf-8")
    template = open( src_template, "r" ).read()
    fh.write( template.replace( "{{ content }}", "\n".join( out ) ) )
    fh.close()

# create texts{{ LANGUAGE }}.h file
#
# LANGUAGE_U    -- language symbol UPPERCASE
# LANGUAGE_C    -- language symbol Capitalized
#
def create_h_file_text_arrays( dst_dot_h_file, template, lang ):
    content = open( template, "r" ).read()
    content = content.replace( "{{ LANGUAGE_U }}", lang.upper() )
    content = content.replace( "{{ LANGUAGE_C }}", lang.capitalize() )

    fh = open( dst_dot_h_file, "w" )
    fh.writelines( content )
    fh.close()

# generic master function for 'lang' translation
# creates texts{{ LANGUAGE }}.c file and texts{{ LANGUAGE }}.h file
def create_lang_files( config, lang, dot_c_template, dot_h_template ):

    #
    # Set language output file names.
    #
    # dst_dot_c_file = "text%s.c" % lang.upper()
    # dst_dot_h_file = "text%s.h" % lang.upper()
    dst_dot_c_file = "../translation/texts%s.c" % lang.upper()
    dst_dot_h_file = "../translation/texts%s.h" % lang.upper()

    #
    # texts{{ lang }}.c
    #
    txt_array = generate_c_file_text_arrays( config, lang, encode_charset )
    if lang == "pl":
        index_arrays = generate_c_file_pl_index_arrays( config, lang )
    else:
        index_arrays = generate_c_file_index_arrays( config, lang )
    write_texts_c_file( dst_dot_c_file, dot_c_template, txt_array, index_arrays, lang )

    #
    # texts{{ lang }}.h
    #
    create_h_file_text_arrays( dst_dot_h_file, dot_h_template, lang )

# create 'common.h' file baseing on "config" data structure
def create_common_h_file( config, dot_h_template, dst_filename ):
    index_array     = generate_common_h_index_array( config )
    write_common_h_file( dst_filename, dot_h_template, index_array )

