.  SOURCE CODE FOR THE XE VERSION OF THE SIC FAMILY OF COMPUTER
0000    SUM       START     0
0000    FIRST     LDX       #0
0003              LDA       #0
0006              +LDB      #TABLE2
000A              BASE      TABLE2
000A    LOOP      ADD       TABLE,X
000D              ADD       TABLE2,X
0010              TIX       COUNT
0013              JLT       LOOP
0016              +STA      TOTAL
001A              RSUB      
001D    MYLIT     +LDA      =C'EOF'
0021    COUNT     RESW      1
0024    TABLE     RESW      2000
1794    TABLE2    RESW      2000
2F04    TOTAL     RESW      1
2F07              *         =C'EOF'
                  END       FIRST
