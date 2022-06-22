#!/bin/python3

import json

def utf2petscii_pl( text ):
    text = list( text )
    pl_array = "ąćęłńóśźżĄĆĘŁŃÓŚŹŻ"
    for i, c in enumerate( text ):
        if c in pl_array:
            text[ i ] = "\\y%x" % ( pl_array.index( c ) + 0x5b )
        elif c == "|":
            text[ i ] = "\\y7e"
        elif c == "[":
            text[ i ] = "\\y1b"
        elif c == "]":
            text[ i ] = "\\y1d"
    return "".join( text )

def underline_text_pl( text, mask ):
    text = list( text )
    pl_array = "ąćęłńóśźżĄĆĘŁŃÓŚŹŻ"
    for i, s2 in enumerate( mask ):
        if s2 == "-":
            s1 = text[ i ]
            if s1 in 'abcdefghijklmnopqrstuvwxyz':
                text[i] = "\\y%x" % ( ord( s1 ) + 32 )    # - 96 + 128
            elif s1 in 'ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789':
                text[i] = "\\y%x" % ( ord( s1 ) + 128 )
            elif s1 == '|':
                text[i] = "\\y%x" % ( 127 )
            elif s1 in pl_array:
                text[i] = "\\y%x" % ( pl_array.index( s1 ) + 0x5b + 128 )
    return "".join( text )

def menu_opt( nr ):
    return "\\y%x" % ( ord( nr ) + 128 )

def generate_common_h_index_array( config ):
    out = []
    common = []
    if config.get( 'main_contents' ):
        if config[ 'main_contents' ].get( 'contents' ):
            for p in config[ 'main_contents' ][ 'contents' ]:
                prefix = 'TXT' if not p.get( 'prefix' ) else p[ 'prefix' ]
                out.append( '    %s_IDX_%s,' % ( prefix, p[ 'id' ] )  )
                if p.get( 'common' ):
                    p1 = 'static const char %s_%s[] =' % ( prefix, p[ 'id' ] )
                    text = utf2petscii_pl( p[ 'common' ] )
                    if p.get( 'common_m' ):     # common text mask label
                        text = underline_text_pl( text, p[ mask ] )
                    if p.get( 'menu_opt' ):
                        text = menu_opt( p.get( "menu_opt" ) ) + text
                    text = utf2petscii_pl( p[ 'common' ] )
                    common.append( '%s s"%s";' % (p1, text))
    out.append( '};\n' )
    out.extend( common )
    out.append( '' )
    return out

def generate_texts_c_generic_lang_file_index_arrays( config, lang ):
    out = []
    for k, v in config.items():
        if v.get( "pragma_label" ):
            out.append( "\n#pragma data ( txt%s%s )" % ( lang.capitalize(), v.get( "pragma_label" ) ) )
        if v.get( "array_label" ):
            out.append( "__export const char* %s_%s[] = {" % ( v.get( "array_label" ), lang.upper() ) )
        for p in v.get( "contents" ):
            prefix = "TXT" if not p.get("prefix") else p["prefix"]
            if p.get( "common" ):
                out.append( '    %s_%s,' % ( prefix, p['id'] )  )
            else:
                out.append( '    %s_%s_%s,' % ( prefix, lang.upper(), p['id'] )  )
        out.append( "};" )
    return out

def generate_texts_c_pl_file_index_arrays( config, lang ):
    out = []
    for k, v in config.items():
        if v.get( "pragma_label" ):
            out.append( "\n#pragma data ( txt%s%s )" % ( lang.capitalize(), v.get( "pragma_label" ) ) )
        if v.get( "array_label" ):
            out.append( "__export const char* %s[] = {" % v.get( "array_label" ) )
        for p in v.get( "contents" ):
            prefix = "TXT" if not p.get("prefix") else p["prefix"]
            if p.get( "common" ):
                out.append( '    %s_%s,' % ( prefix, p['id'] )  )
            else:
                out.append( '    %s_%s_%s,' % ( prefix, lang.upper(), p['id'] )  )
        out.append( "};" )
    return out

def generate_c_file_text_arrays( config, lang, text_filter ):
    out = []
    for k, v in config.items():
        for p in v.get( "contents" ):
            prefix = "TXT" if not p.get("prefix") else p["prefix"]
            if p.get( "common" ):
                # this definition is already in common.h file
                # so we will get index from common.h file
                # in index array part
                pass
            else:
                p1 = 'const char %s_%s_%s[] = ' %( prefix, lang.upper(), p['id'] )
                text = p[ lang ]
                mask = "%s_m" % lang
                if p.get( mask ):
                    text = underline_text_pl( text, p[ mask ] )
                if p.get( "menu_opt" ):
                    text = menu_opt( p.get( "menu_opt" ) ) + text
                text = text_filter( text )
                out.append( f"{p1} s\"{text}\";" )
    return out

def read_json_config_file( filename ):
    fh = open("data.json", "r")
    content = json.load(fh)
    fh.close()
    return content

def write_common_h_file( index_array ):
    fh = open("common.h", "w")
    template = open( "templates/common_h.template", "r" ).read()
    fh.write( template.replace( "{{ content }}", "\n".join( index_array ) ) )
    fh.close()

def write_texts_c_file( template, text_array, index_array, lang ):
    out = []
    out.append( "#pragma data ( txt%sTxtData )\n" % lang.capitalize() )
    out.append( "\n".join( text_array ) )
    out.append( "\n".join( index_array ) )

    filename = "texts%s.c" % lang.upper()
    fh = open(filename, "w")
    template = open( template, "r" ).read()
    fh.write( template.replace( "{{ content }}", "\n".join( out ) ) )
    fh.close()

def create_h_file_text_arrays( template, lang ):
    content = open( template, "r" ).read()
    content = content.replace( "{{ LANGUAGE_U }}", lang.upper() )
    content = content.replace( "{{ LANGUAGE_C }}", lang.capitalize() )

    filename = "texts%s.h" % lang.upper()
    fh = open( filename, "w" )
    fh.writelines( content )
    fh.close()

def create_lang_files( config, lang ):
    # textsPL.c
    txt_array = generate_c_file_text_arrays( config, lang, utf2petscii_pl )
    index_arrays = generate_texts_c_generic_lang_file_index_arrays( config, lang )
    write_texts_c_file( "templates/texts_c.template", txt_array, index_arrays, lang )
    # textsPL.h
    create_h_file_text_arrays( "templates/texts_h.template", lang )

def create_pl_files( config ):
    # textsPL.c
    txt_array = generate_c_file_text_arrays( config, "pl", utf2petscii_pl )
    index_arrays = generate_texts_c_pl_file_index_arrays( config, "pl" )
    write_texts_c_file(  "templates/texts_c_pl.template", txt_array, index_arrays, "pl" )
    # textsPL.h
    create_h_file_text_arrays(  "templates/texts_h_pl.template", "pl" )

def create_common_h_file( config ):
    index_array = generate_common_h_index_array( config )
    write_common_h_file( index_array )

if __name__ == "__main__":

    config = read_json_config_file( "data.json" )
    create_common_h_file( config )
    create_pl_files( config )
    create_lang_files( config, "en" )









