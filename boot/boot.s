;<宋傲>

;===============================Multiboot Standard===============================
;==============请参阅http://www.uruk.org/orig-grub/boot-proposal.html============

MBOOT_HEADER_MAGIC 	equ 	0x1BADB002 	; Multiboot 的魔数，用来标识一个可执行内核

;flags
MBOOT_PAGE_ALIGN 	equ 	1 << 0    	; 内核按页面大小4KB对齐
MBOOT_MEM_INFO 		equ 	1 << 1    	; mem_* 结构体中含有可用内存信息，即返回的结构体包含内存信息
						

; 把上述两位进行拼接，形成flags
MBOOT_HEADER_FLAGS 	equ 	MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO

; 求和校验
; (即magic + flags + checksum = 0)
MBOOT_CHECKSUM 		equ 	- (MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

;   +-------------------+
;0	| magic: 0x1BADB002 |	(required)
;4	| flags		        |	(required)
;8	| checksum	        |	(required)
;	+-------------------+

;-----------------------------------------------------------------------------

[BITS 32]  	; 汇编时按32位处理

section .init.text 	; 临时代码段从这里开始

;把header放在这个位置

dd MBOOT_HEADER_MAGIC 	
dd MBOOT_HEADER_FLAGS   
dd MBOOT_CHECKSUM       

[GLOBAL start] 		;全局的入口标签，在链接脚本中指明就可在这个位置启动内核
[GLOBAL mboot_ptr_tmp] 	; multiboot返回信息结构体的指针
[EXTERN kern_entry] 	; 内核 C 代码的入口

start:
	cli  				; 关闭中断
	mov [mboot_ptr_tmp], ebx	; 按照规定：ebx存放返回信息结构体的指针  mboot_ptr_tmp 变量存入这个指针，后面会用到
	mov esp, STACK_TOP  		; 内核用到的栈
	and esp, 0FFFFFFF0H			; 栈地址对齐到16位
	mov ebp, 0 			        
    
	call kern_entry	; 调用C代码的入口，转入C代码对应的部分执行


;=========================================================================

section .init.data		;  进入保护模式前使用的数据段
stack:    times 1024 db 0  	; 
STACK_TOP equ $-stack-1 	; 

mboot_ptr_tmp: dd 0		; 


;</宋傲>