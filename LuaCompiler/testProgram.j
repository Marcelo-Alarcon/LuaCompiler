.class public testProgram
.super java/lang/Object

.field private static _sysin Ljava/util/Scanner;
.field private static result I
.field private static result1 I
.field private static result2 I

;
; Runtime input scanner
;
.method static <clinit>()V

	new	java/util/Scanner
	dup
	getstatic	java/lang/System/in Ljava/io/InputStream;
	invokespecial	java/util/Scanner/<init>(Ljava/io/InputStream;)V
	putstatic	testProgram/_sysin Ljava/util/Scanner;
	return

.limit locals 0
.limit stack 3
.end method

;
; Main class constructor
;
.method public <init>()V
.var 0 is this LtestProgram;

	aload_0
	invokespecial	java/lang/Object/<init>()V
	return

.limit locals 1
.limit stack 1
.end method

;
; FUNCTION testData
;
.method private static testData()I

.var  is testData I
.var  is x I
.var  is y I
.var  is z I
;
; ASSIGNMENT
;
	iload_0
	istore_0
;
; ASSIGNMENT
;
	iload_1
	istore_0
;
; ASSIGNMENT
;
	iconst_0
	istore_0
;
; REPEAT
;
L001:
;
; ASSIGNMENT
;
	iload_0
	iconst_4
	iconst_2
	imul
	isub
	istore_0
;
; ASSIGNMENT
;
	iload_0
	iconst_2
	idiv
	iconst_3
	isub
	istore_0
;
; ASSIGNMENT
;
	iload_0
	iconst_1
	iadd
	istore_0
;
; UNTIL
;
;
; z<5
;
	iload_0
	iconst_5
	if_icmplt	L003
	iconst_0
	goto	L004
L003:
	iconst_1
L004:
	ifne	L002
	goto	L001
L002:
;
; IF
;
;
; x<y
;
	iload_0
	iload_0
	if_icmplt	L007
	iconst_0
	goto	L008
L007:
	iconst_1
L008:
	ifeq	L006
	iconst_1
L006:
;
; ELSE IF
;
;
; x>y
;
	iload_0
	iload_0
	if_icmpgt	L009
	iconst_0
	goto	L010
L009:
	iconst_1
L010:
	ifeq	L006
	iconst_0
L006:
;
; ELSE
;
	goto	L005
L006:
L005:

	istore_2
	iload_2
	ireturn

.limit locals 3
.limit stack 3
.end method

;
; MAIN
;
.method public static main([Ljava/lang/String;)V
.var 0 is args [Ljava/lang/String;
.var 1 is _start Ljava/time/Instant;
.var 2 is _end Ljava/time/Instant;
.var 3 is _elapsed J

	invokestatic	java/time/Instant/now()Ljava/time/Instant;
	astore_1

;
; ASSIGNMENT
;
;
; FUNCTION CALL
;
	invokestatic	testProgram/testData(II)I
	putstatic	testProgram/result1 I
;
; ASSIGNMENT
;
;
; FUNCTION CALL
;
	invokestatic	testProgram/testData(II)I
	putstatic	testProgram/result2 I
;
; PRINT
;
	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"%b\n"
	iconst_1
	anewarray	java/lang/Object
	dup
	iconst_0
	ldc	"TEST"
	aastore
	invokevirtual	java/io/PrintStream/printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop
;
; PRINT
;
	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"%b\n"
	iconst_1
	anewarray	java/lang/Object
	dup
	iconst_0
;
; FUNCTION CALL
;
	invokestatic	testProgram/testData(II)I
	aastore
	invokevirtual	java/io/PrintStream/printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop
;
; END MAIN
;

	invokestatic	java/time/Instant/now()Ljava/time/Instant;
	astore_2
	aload_1
	aload_2
	invokestatic	java/time/Duration/between(Ljava/time/temporal/Temporal;Ljava/time/temporal/Temporal;)Ljava/time/Duration;
	invokevirtual	java/time/Duration/toMillis()J
	lstore_3
	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"\n[%,d milliseconds execution time.]\n"
	iconst_1
	anewarray	java/lang/Object
	dup
	iconst_0
	lload_3
	invokestatic	java/lang/Long/valueOf(J)Ljava/lang/Long;
	aastore
	invokevirtual	java/io/PrintStream/printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop

	return

.limit locals 8
.limit stack 6
.end method
