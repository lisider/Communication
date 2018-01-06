.PHONY:all

all:client server

client:client.c comm.c       #gcc -o $@（目标文件） $^（依赖关系:后的所有内容）
	gcc -o $@ $^
server:server.c comm.c
	gcc -o $@ $^

.PHONY:clean

clean:
	rm -f server client
