select r1 || '*' || r1 || '=' || r1 * r1 A,
       decode(r2, '', '', r2 || '*' || r1 || '=' || r2 * r1) b,
       decode(r3, '', '', r3 || '*' || r1 || '=' || r3 * r1) C,
       decode(r4, '', '', r4 || '*' || r1 || '=' || r4 * r1) D,
       decode(r5, '', '', r5 || '*' || r1 || '=' || r5 * r1) E,
       decode(r6, '', '', r6 || '*' || r1 || '=' || r6 * r1) F,
       decode(r7, '', '', r7 || '*' || r1 || '=' || r7 * r1) G,
       decode(r8, '', '', r8 || '*' || r1 || '=' || r8 * r1) H,
       decode(r9, '', '', r9 || '*' || r1 || '=' || r9 * r1) I
from   (select level r1,
               lag(level, 1) over(order by level) r2,
               lag(level, 2) over(order by level) r3,
               lag(level, 3) over(order by level) r4,
               lag(level, 4) over(order by level) r5,
               lag(level, 5) over(order by level) r6,
               lag(level, 6) over(order by level) r7,
               lag(level, 7) over(order by level) r8,
               lag(level, 8) over(order by level) r9
        from   dual
        connect by level < 10)

;
