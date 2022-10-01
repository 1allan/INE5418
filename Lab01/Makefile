server:
	gcc ./servidor.c -o ./out/s1_bingus
	./out/s1_bingus

client:
	gcc ./cliente.c -o ./out/c1_bingus
	./out/c1_bingus

stop:
	pkill s1_bingus
	pkill c1_bingus
	rm -f ./server_socket

rebuild: stop server
