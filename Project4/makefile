#注释来源：陈皓：跟我一起写makefile；来源：https://blog.csdn.net/haoel/article/details/2886
#makefile的命令，必须要从Tab键开始。
objects = main.o lab4.o
lab4 : $(objects)
	cc -o lab4 $(objects)

#只要 make 看到一个[.o]文件，它就会自动的把[.c]文件加在依赖关系中
main.o :  lab4.c lab4.h
lab4.o : lab4.h
#PHONY表示clean 是一个伪目标，在 rm 命令前面加了一个小减号的意思就是，也许某些文件出现问题，但不要管，继续做后面的事。
clean:
	-rm lab4 $(objects)