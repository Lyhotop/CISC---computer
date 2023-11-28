	lw	0	1	x1
	lw	0	2	x2
	lw	0	3	x3
	lw	0	4	x4
	lw	0	5	res1
	lw	0	6	res2
	lw	0	7	res3
	#div	3	4	5
	#xor	3	4	6
	#ximul	3	4	7
	halt	
x1	.fill	54
x2	.fill	6	
x3	.fill	1
x4	.fill	2
res1	.fill	5
res2	.fill	6
res3	.fill	7