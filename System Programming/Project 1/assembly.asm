segment .data
	black	equ 	0
	white	equ	255

segment .bss
	row_end		resd	1 	; end of row
	array_end	resd	1	; end of array address

segment  .text
	global brightness
	global contrast
	global average
	global grow
	global shrink
	global blur
	global invert
	global edge
	global atleast
	global clear

check_boundary:
	cmp	word cx, 255		; if result is bigger than 255
	jg	check_boundary_bigger
	cmp	word cx, 0		; if result is lower than 0
	jl	check_boundary_lower
	mov	[eax], cl		; set result
	jmp	check_boundary_next
check_boundary_bigger:
	mov	byte [eax], 255		; set 255
	jmp	check_boundary_next
check_boundary_lower:
	mov	byte [eax], 0		; set 0
check_boundary_next:
	ret

brightness:
	push	ebp
	mov	ebp, esp

	; ebp+8 = image, ebp+12 = width, ebp+16 = height, ebp+20 = value
	mov	eax, [ebp+12]
	mul	dword [ebp+16]
	add	eax, [ebp+8]
	mov	[array_end], eax	; array_end = image + height * width

	mov	eax, [ebp+8]		; eax = image
brightness_loop:
	xor	cx, cx			; clear cx
	mov	cl, [eax]		; add pixel to cl, because of pixel is unsigned 8bit(cl), it is extended to signed 16bit(cx)
	add	cx, [ebp+20]		; add value to pixel

	call	check_boundary
	inc	eax			; eax++
	cmp	eax, [array_end]
	jne	brightness_loop

	pop	ebp
	ret

contrast:
	push	ebp
	mov	ebp, esp

	; ebp+8 = image, ebp+12 = width, ebp+16 = height, ebp+20 = value
	mov	eax, [ebp+12]
	mul	dword [ebp+16]
	add	eax, [ebp+8]
	mov	[array_end], eax	; array_end = image + height * width
	
	mov	eax, [ebp+8]		; eax = image
contrast_loop:
	xor	ecx, ecx		; clear ecx
	mov	cl, [eax]
	imul	dword ecx, [ebp+20]
	shr	ecx, 6			; ecx = pixel * value / 64
	
	call	check_boundary
	inc	eax			; eax++
	cmp	eax, [array_end]
	jne	contrast_loop

	pop	ebp
	ret	

average:
	push	ebp
	mov	ebp, esp

	; ebp+8 = image1, ebp+12 = image2, ebp+16 = width, ebp+20 = height
	mov	eax, [ebp+16]
	mul	dword [ebp+20]
	add	eax, [ebp+8]
	mov	[array_end], eax	; array_end = image + height * width
	
	mov	eax, [ebp+8]		; eax = image1
	mov	ebx, [ebp+12]		; eax = image2
average_loop:
	xor	ecx, ecx		; clear ecx
	mov	cx, [eax]
	add	cx, [ebx]
	inc	cx
	shr	cx, 1			; cx = (pixel of image1 + pixel of image2 + 1) / 2
	
	call	check_boundary
	inc	eax			; eax++
	inc	ebx			; ebx++
	cmp	eax, [array_end]
	jne	average_loop

	pop	ebp
	ret
	
grow:
	push	ebp
	mov	ebp, esp

	xor	eax, eax
	xor	ebx, ebx
	xor	ecx, ecx
	xor	edx, edx

	; ebp+8 = image1, ebp+12 = image2, ebp+16 = width, ebp+20 = height
	mov	eax, [ebp+8]
	add	eax, [ebp+16]
	mov	[row_end], eax		; row_end = image1 + width
	mov	eax, [ebp+16]
	mul	dword [ebp+20]
	add	eax, [ebp+8]
	mov	[array_end], eax	; array_end = image1 + height * width

	mov	eax, [ebp+8]		; eax = image1
	mov	ebx, [ebp+12]		; eax = image2
grow_while:
	mov	ecx, [eax]
	mov	[ebx], ecx
	inc	ebx
	mov	[ebx], ecx
	add	ebx, [ebp+16]
	add	ebx, [ebp+16]
	mov	[ebx], ecx
	dec	ebx
	mov	[ebx], ecx
	inc	eax
	add	dword ebx, 2

	cmp	eax, [row_end]		; is row_end?
	jne	grow_while_then
	mov	ecx, [ebp+16]
	add	[row_end], ecx		; increase row_end with width

	cmp eax, [array_end]		; if row_end, it can be array_end too
	je grow_while_end
	jmp grow_while

grow_while_then:			; decrease ebx with 2 * width
	sub ebx, [ebp+16]
	sub ebx, [ebp+16]
	jmp grow_while
grow_while_end:

	pop	ebp			; adjust stack frame
	ret

shrink:
	push	ebp
	mov	ebp, esp

	; handle parameters
	mov	edx, [ebp+8]		; edx <= address of img1
	mov	ecx, [ebp+12]		; ecx <= address of img2
	mov	ebx, [ebp+16]		; ebx <= width
	mov	eax, [ebp+20]		; eax <= height

	imul	eax, ebx		; eax <= size
	add	eax, edx		; eax <= end of img1
	mov	[array_end], eax
	xor	eax, eax
	mov	eax, edx		; eax  <= base addr
	add	eax, ebx		; eax += offset (end of row)
	mov	[row_end], eax		; row_end <= eax

shrink_loop1:
	; copy every 2nd pixel into new image location
	xor	eax, eax
	mov	al, byte [edx]		; al <= pixel value
	mov	byte [ecx], byte al	; img2 <= al (pixel)
	inc	ecx			; increment new img 1 pixel
	add	edx, 2			; increment orig img 2 pixels
	cmp	edx, [row_end]		; check if end of row is reached
	jne	shrink_loop1		; if not, jump back to loop

	; if end of line is reached
	add	[row_end], ebx		; row_end += width (upper row(even))
	mov	edx, [row_end]		; fix 1st img ptr (skip odd row)
	add	[row_end], ebx		; row_end += width (upper row(odd))
	mov	eax, [row_end]		; eax = row end (for comparison)
	cmp	eax, dword [array_end]	; check if boundary reached
	jge	shrink_end		; if boundary reached, end
	jmp	shrink_loop1		; if not, continue loop

shrink_end:
	pop	ebp		; restore stack frame
	ret			; return


blur:
	push	ebp
	mov	ebp, esp

	; handle parameters
	mov	edx, [ebp+8]	; edx = array address
	mov	ecx, [ebp+12] 	; ecx = width
	mov	ebx, [ebp+16]	; ebx = height
	imul	ebx, ecx	; ebx = array length
	add	ebx, edx	; ebx = end of array address
	xor	eax, eax	; eax = 0

	; determine the current end of row address
	mov	eax, edx	; base address
	mov	[row_end], eax	; save base address
	add	[row_end], cx	; add the offset (now points to upper row)

blur_loop:
	; sum 4 pixels (2x2) in al
	push	edx		; save main index of array
	mov	al, byte [edx]	; 1st pixel (1,1)
	inc	edx		; next pixel
	add	al, byte [edx]	; 2nd pixel (1,2)
	dec	edx		; previos pixel
	add	edx, ecx	; pixel above
	add	al, byte [edx]	; 3rd pixel (2,1)
	inc	edx		; next pixel
	add	al, byte [edx]	; 4th pixel (2,2)
	shr	al, 2		; average of 4 pixels
	pop	edx		; restore current array index

	; apply blur to 2x2
	push 	edx		; save index
	mov	byte [edx], al	; blur 1st pixel (1,1)
	inc	edx		; move to next pixel
	mov	byte [edx], al	; blue 2nd pixel (1,2)
	dec	edx		; move back to 1st pixel
	add	edx, ecx	; move to upper pixel
	mov	byte [edx], al	; blur 3rd pixel (2,1)
	inc	edx		; move to next pixel
	mov	byte [edx], al	; blur 4th pixel (2,2)
	pop	edx		; restore current array index

	; iterate
	xor	eax, eax	; clear eax
	add	edx, 2		; move to next frame (2 steps)
	cmp	edx, [row_end]  ; if its now end of the row
	jne	blur_loop	; jump back to the loop

	; if its the end of the row
	add	[row_end], ecx	; determine new row end (upper row)
	push 	eax		; save eax
	mov	eax, [row_end]	; check if new row end is end of image
	cmp	eax, ebx	; compare addresses
	jge	blur_end	; if end of image is reached jump to the end
	pop	eax		; if not, restore eax
	jmp	blur_loop	; jump back to loop

blur_end:
	pop	eax		; eax was pushed, so, pop back.
	pop 	ebp
	ret

edge:
	push 	ebp
	mov	ebp, esp

	; handle parameters
	mov 	edx, [ebp+8]	; edx = address
	mov	ecx, [ebp+12]	; ecx = width
	mov	ebx, [ebp+16]	; ebx = height
	xor	eax, eax	; eax = 0

	; make the top row black
	imul	ebx, ecx	; ebx = last pixel index
	add	edx, ebx	; edx -> last pixel
	mov	eax, edx	; eax = last pixel address
	sub	eax, ecx	; eax = end of loop address
	dec	eax		; include last pixel too
	mov	[array_end], eax
edge_loop1:
	mov	[edx], byte black	; make pixel black
	dec	edx			; index--
	cmp	edx, eax		; if last address not reached
	jne	edge_loop1		; jump back to loop


	; edge other pixels
	mov	edx, [ebp+8]	; restore array address
	mov	ecx, [ebp+8]	; ecx = address of upper pixel
	mov	ebx, [ebp+12]	; ebx = width (for upper pixel)
	add	ecx, ebx	; ecx += offset -> upper pix

edge_loop2:
	xor	eax, eax	; clear eax
	mov	al, byte [edx]	; al = value in base address
	cmp	byte [ecx], al	; compare al with upper pixel
	jge	_black		; jump to white if upper pixel is greater

_white:
	mov	[edx], byte white	; make pixel white
	jmp	edge_cont		; continue

_black:
	mov	[edx], byte black	; make pixel black
	jmp	edge_cont		; continue

edge_cont:
	inc	edx		; increment index of base addr
	inc	ecx		; increment upper pixels index
	mov	eax, edx	; eax <= base addr
	cmp	eax, [array_end]; compare base addr with array end
	jge	edge_end	; jump to end if array boundary is reached
	jmp	edge_loop2	; continue loop otherwise

edge_end:
	pop 	ebp		; restore stack frame
	ret			; return


invert:
	push	ebp
	mov	ebp, esp

	; handle parameters
	mov	edx, [ebp+8]		; edx = array address
	mov	ebx, [ebp+12]		; ebx = height
	mov	ecx, [ebp+16]		; ecx = width

	imul	ebx, ecx		; ebx <- total length of array
	xor	eax, eax		; clear eax

invert_loop:
	mov	al, white		; eax = 255
	sub	al, byte [edx]		; eax -= arr[i]
	mov	[edx], al		; arr[i] = eax
	dec	ebx			; loop counter (size--)
	inc	edx			; index++
	cmp	ebx, 0			; if size is not reached
	jne	invert_loop		; iterate loop

	xor	eax, eax		; clear eax
	pop	ebp			; adjust stack frame
	ret				; return

atleast:
	push	ebp
	mov	ebp, esp

	; handle parameters
	mov	edx, [ebp+8]		; edx <= img address
	mov	ecx, [ebp+12]		; ecx <= width
	mov	eax, [ebp+16]		; eax <= height
	imul	ecx, eax		; ecx <= total length of array
	dec	ecx
	add	edx, ecx		; edx points to last pixel now
	xor	ebx, ebx
	mov	ebx, [ebp+20]		; ebx <= value
	xor	eax, eax		; clear eax

atleast_loop:
	cmp	[edx], bl	; compare pixel with the value
	jge	atleast_greater

atleast_cont:
	dec	ecx		; decrement loop counter
	dec	edx		; decrement index
	cmp	ecx, 0		; is loop counter 0?
	je	atleast_end	; if yes, end the loop
	jmp	atleast_loop	; if not, continue looping

atleast_greater:
	inc	eax
	jmp	atleast_cont

atleast_end:
	pop	ebp
	ret

clear:
	push	ebp
	mov	ebp, esp

	; handle paramters
	mov	edx, [ebp+8]	; edx <= img address
	mov	eax, [ebp+12]	; eax <= width
	mov	ecx, [ebp+16]	; ecx <= height
	xor	ebx, ebx	; clear ebx
	mov	ebx, [ebp+20]	; bl <= value (0-255)
	imul	ecx, eax	; ecx <= total length of array

	add	edx, ecx	; edx points to last pixel now
clear_loop:
	mov	[edx], bl	; set the pixel value
	dec	edx		; decrement pointer
	dec	ecx		; decrement loop counter
	cmp	ecx, 0		; if its not 0
	jne	clear_loop	; continue loop

	pop 	ebp		; restore stack frame
	ret			; return
