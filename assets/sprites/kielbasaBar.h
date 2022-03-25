// 8 sprites generated with spritemate on 24/09/2021, 20:33:17
// Byte 64 of each sprite contains multicolor (high nibble) & color (low nibble) information

// LDA #$08 // sprite multicolor 1
// STA $D025
// LDA #$0a // sprite multicolor 2
// STA $D026


__address(0x0a00) __export char SPR_KIELBASA_BAR[64*8] = kickasm {{

    // sprite 0 / multicolor / color: $09
    .byte $00,$aa,$00,$01,$7d,$80,$05,$ff
    .byte $68,$87,$ff,$da,$47,$ff,$da,$6f
    .byte $ff,$fa,$9f,$ff,$f9,$af,$ff,$fb
    .byte $27,$ff,$f7,$2a,$ff,$ff,$2a,$7f
    .byte $ff,$2a,$9f,$ff,$0a,$ab,$ff,$0a
    .byte $aa,$ff,$02,$aa,$95,$02,$aa,$aa
    .byte $00,$aa,$aa,$00,$0a,$aa,$00,$00
    .byte $aa,$00,$00,$0a,$00,$00,$00,$89

    // sprite 1 / multicolor / color: $09
    .byte $00,$00,$00,$00,$00,$00,$00,$00
    .byte $00,$00,$00,$00,$a0,$00,$00,$e8
    .byte $00,$00,$d9,$aa,$80,$f6,$96,$aa
    .byte $fe,$7d,$a5,$fe,$ff,$9f,$fe,$ff
    .byte $bf,$ff,$ff,$bf,$ff,$ff,$7f,$ff
    .byte $ff,$ff,$ff,$ff,$ff,$57,$ff,$ff
    .byte $a9,$5f,$ff,$aa,$95,$ff,$aa,$a9
    .byte $57,$aa,$aa,$aa,$0a,$aa,$aa,$89

    // sprite 2 / multicolor / color: $09
    .byte $00,$00,$00,$00,$00,$00,$00,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00
    .byte $00,$00,$02,$02,$80,$aa,$aa,$aa
    .byte $5a,$79,$bb,$f6,$ff,$bf,$fd,$ff
    .byte $7f,$fd,$ff,$7f,$ff,$ff,$ff,$ff
    .byte $ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff
    .byte $ff,$ff,$ff,$ff,$ff,$ff,$57,$ff
    .byte $55,$a5,$d5,$6a,$aa,$aa,$aa,$89

    // sprite 3 / multicolor / color: $09
    .byte $00,$00,$00,$00,$00,$00,$00,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00
    .byte $00,$00,$02,$02,$80,$aa,$aa,$aa
    .byte $5a,$79,$bb,$f6,$ff,$bf,$fe,$ff
    .byte $bf,$fe,$ff,$ff,$fd,$ff,$ff,$ff
    .byte $ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff
    .byte $ff,$ff,$ff,$ff,$ff,$ff,$55,$5f
    .byte $55,$aa,$95,$6a,$aa,$aa,$aa,$89

    // sprite 4 / multicolor / color: $09
    .byte $00,$00,$00,$00,$00,$00,$00,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00
    .byte $00,$00,$02,$80,$80,$aa,$aa,$aa
    .byte $ee,$6d,$a5,$fe,$ff,$9f,$fe,$ff
    .byte $bf,$ff,$ff,$bf,$ff,$ff,$7f,$ff
    .byte $ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff
    .byte $ff,$ff,$ff,$ff,$ff,$ff,$55,$f5
    .byte $55,$a9,$56,$aa,$aa,$aa,$aa,$89

    // sprite 5 / multicolor / color: $09
    .byte $00,$00,$00,$00,$00,$00,$00,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$00
    .byte $00,$00,$0a,$80,$80,$aa,$aa,$aa
    .byte $ee,$6d,$a5,$fe,$bf,$9f,$fe,$ff
    .byte $7f,$fd,$ff,$bf,$ff,$ff,$7f,$ff
    .byte $ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff
    .byte $ff,$ff,$ff,$fd,$57,$ff,$55,$a5
    .byte $55,$aa,$aa,$aa,$aa,$aa,$aa,$89

    // sprite 6 / multicolor / color: $09
    .byte $00,$00,$00,$00,$00,$00,$00,$00
    .byte $00,$00,$00,$02,$00,$00,$0b,$00
    .byte $0a,$ab,$00,$aa,$a7,$aa,$9f,$9f
    .byte $5a,$9f,$9f,$f6,$7f,$bf,$fe,$ff
    .byte $bf,$fe,$ff,$ff,$fe,$ff,$ff,$fd
    .byte $ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff
    .byte $ff,$ff,$5a,$ff,$da,$aa,$da,$aa
    .byte $aa,$aa,$aa,$a0,$aa,$a0,$00,$89

    // sprite 7 / multicolor / color: $09
    .byte $02,$aa,$00,$0a,$7d,$80,$99,$ff
    .byte $60,$ab,$ff,$e2,$fb,$ff,$d2,$fb
    .byte $ff,$d9,$fb,$ff,$f9,$f7,$ff,$fa
    .byte $ff,$ff,$d8,$ff,$ff,$58,$ff,$fd
    .byte $a0,$ff,$fe,$a0,$ff,$5a,$80,$fd
    .byte $aa,$80,$fa,$aa,$00,$aa,$a8,$00
    .byte $aa,$a0,$00,$aa,$00,$00,$a0,$00
    .byte $00,$00,$00,$00,$00,$00,$00,$89
}};