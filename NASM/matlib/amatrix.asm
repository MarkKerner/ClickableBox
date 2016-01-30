; ----------------------------------------------------
; amatrix.asm
; @brief	OpenGL matrix functions
; @author	Mark Kerner
; @date		23.01.2016
; ----------------------------------------------------
section .data
	printd db 'Print: %f', 10, 0
	printvec3 db 'x: %d, y: %d, z: %d', 10, 0
	zfone db 'ZF = 1\n', 0
	zfzero db 'ZF = 0\n', 0
	align 4
	testint dd 10

section .text

struc vec3
	.x resd 1
	.y resd 1
	.z resd 1
endstruc

global avec3_subtract
global avec3_assign

global amat4_identity
global amat4_scale
global amat4_translation
extern printf

;void vec3_print(int x, int y, int z);
avec3_print:
	push ebp
	mov ebp, esp

	;Print received arguments
	push dword[ebp + 16]
	push dword[ebp + 12]
	push dword[ebp + 8]
	push printvec3
	call printf
	add esp, 16

	mov esp, ebp
	pop ebp

	ret

;void vec3_subtract(vec3* pOut, vec3* pV1, vec3* pV2);
avec3_subtract:
	push ebp
	mov ebp, esp

	;; read arguments to registers
	mov eax, [ebp + 8] ;; *pOut
	mov ecx, [ebp + 12] ;; *pV1
	mov edx, [ebp + 16] ;; *pV2

	;; move vectors to registers
	movups xmm0, [ecx]
	movups xmm1, [edx]

	subps xmm0, xmm1 ;; subtract vectors

	movlps qword[eax + vec3.x], xmm0 ;; write xmm0 xy values
	movhlps xmm0, xmm0 ;; shuffle move xmm0 zw to xy
	movss dword[eax + vec3.z], xmm0 ;; write z value

	mov esp, ebp
	pop ebp

	ret

;void vec3_assign(vec3* pOut, vec3* pV);
avec3_assign:
	push ebp
	mov ebp, esp

	;; read arguments to registers
	mov eax, [ebp + 8] ;;*pOut
	mov ecx, [ebp + 12] ;;*pV

	movlps   xmm0, qword[ecx + vec3.x]  ;; pV.xy
	movss    xmm1, dword[ecx + vec3.z]  ;; pV.z
	movlhps xmm0, xmm1 ;; xmm0.zw = xmm1.xy

	movlps qword[eax + vec3.x], xmm0 ;; write xmm0 xy values
	movhlps xmm0, xmm0 ;; shuffle move xmm0 zw to xy
	movss dword[eax + vec3.z], xmm0 ;; write z value

	mov esp, ebp
	pop ebp

	ret

;;void mat4_scale(float pOut[16], const float x, const float y, const float z);
amat4_scale:
	push ebp
	mov ebp, esp
	push ebx

	mov edx, [ebp + 8] ;;*pOut

	mov eax, [ebp + 12] ;;x
	mov ebx, [ebp + 16] ;;y
	mov ecx, [ebp + 20] ;;z

	xorps xmm0, xmm0 ;; 0000

	;set all matrix values to 0.0
	movups [edx], xmm0 
	movups [edx + 4*4], xmm0 
	movups [edx + 4*8], xmm0 
	movups [edx + 4*12], xmm0

	;set first 3 diagonal values to xyz
	mov dword[edx + 4*0], eax
	mov dword[edx + 4*5], ebx
	mov dword[edx + 4*10], ecx

	;set last diagonal value to 1.0
	fld1
	fstp dword[edx + 4*15]

	pop ebx
	mov esp, ebp
	pop ebp

	ret

;;void mat4_translation(float pOut[16], const float x, const float y, const float z);
amat4_translation:
	push ebp
	mov ebp, esp
	push ebx

	mov edx, [ebp + 8] ;;*pOut

	mov eax, [ebp + 12] ;;x
	mov ebx, [ebp + 16] ;;y
	mov ecx, [ebp + 20] ;;z

	xorps xmm0, xmm0 ;; 0000

	;; set all matrix values to 0.0
	movups [edx], xmm0 
	movups [edx + 4*4], xmm0 
	movups [edx + 4*8], xmm0 
	movups [edx + 4*12], xmm0 

	;; set diagonal values to 1.0
	fld1
	fst dword[edx + 4*0]
	fst dword[edx + 4*5]
	fst dword[edx + 4*10]
	fstp dword[edx + 4*15]


	mov dword[edx + 4*12], eax
	mov dword[edx + 4*13], ebx
	mov dword[edx + 4*14], ecx

	pop ebx
	mov esp, ebp
	pop ebp

	ret

;; void mat4_identity(float pOut[16])
amat4_identity:
	push ebp
	mov ebp, esp

	mov edx, [ebp + 8] ;;*pOut

	xorps xmm0, xmm0 ;; 0000

	;; set all matrix values to 0.0
	movups [edx + 4*0], xmm0 
	movups [edx + 4*4], xmm0 
	movups [edx + 4*8], xmm0 
	movups [edx + 4*12], xmm0 

	;; set diagonal values to 1.0
	fld1
	fst dword[edx + 4*0]
	fst dword[edx + 4*5]
	fst dword[edx + 4*10]
	fstp dword[edx + 4*15]

	mov esp, ebp
	pop ebp

	ret