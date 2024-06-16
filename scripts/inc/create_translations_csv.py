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

def generate_godot_localization_file( config ):
    out = ["key,en,pl"]
    for k, v in config.items():
        for p in v.get( "contents" ):


            if p.get( "common" ):
                m_id = p['id']
                m_en = p['common']
                m_pl = p['common']
            else:
                m_id = p['id']
                m_en = p['en']
                m_pl = p['pl']

            if type(m_en) == list:
                m_en = '\\n'.join(m_en)
            if type(m_pl) == list:
                m_pl = '\\n'.join(m_pl)

            # add appropriate menu option symbol if this is menu option
            if p.get( "menu_opt" ):
                # m_nr = menu_opt( p.get( "menu_opt") )
                m_nr = p.get( "menu_opt")
                m_en = m_nr + ". " + m_en
                m_pl = m_nr + ". " + m_pl

            result = '{0},"{1}","{2}"'.format( m_id, m_en, m_pl )
            out.append( result )
            # print( result )

    return '\n'.join(out)

def save_data( content, filename ):
    with open(filename, 'w', encoding="utf-8") as fh:
        fh.write( content )










