	lw	0	1	x1
	lw	0	2	x2
	lw	0	3	x3
	lw	0	4	x4
	jmae	1	2	pointA
	add	1	2	6
pointA  xor	1	2	7
	jmnge	3	4	pointB
	add	3	4	8
pointB  xor	3	4	9
	halt
x1	.fill	10
x2	.fill	5	
x3	.fill	7
x4	.fill	4