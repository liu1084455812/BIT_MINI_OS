;   <林理露>
;
;   switch_to.s
;   bit_miniOS
;
;   Created by 林理露 on 23/05/2017.
;   Copyright © 2017 林理露. All rights reserved.


[global switch_to]

; 具体的线程切换操作，寄存器的保存与恢复
switch_to:
        mov eax, [esp+4]

        mov [eax+0],  esp
        mov [eax+4],  ebp
        mov [eax+8],  ebx
        mov [eax+12], esi
        mov [eax+16], edi
        pushf
        pop ecx
        mov [eax+20], ecx

        mov eax, [esp+8]

        mov esp, [eax+0]
        mov ebp, [eax+4]
        mov ebx, [eax+8]
        mov esi, [eax+12]
        mov edi, [eax+16]
        mov eax, [eax+20]
        push eax
        popf
 	
        ret

;   </林理露>
