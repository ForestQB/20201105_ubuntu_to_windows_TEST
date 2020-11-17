all : helloworld.c
	gcc -Wall -g -o main helloworld.c -lpthread 
#	gcc -Wall -g -o server server.c
#	-o main 創建main啟動檔 
#	all : helloworld.c 讀取helloworld.c檔
#	all : helloworld.c \換行可輸入 新的讀入.c
clean :
	rm main
#	rm server
#	rm main 砍掉main啟動檔