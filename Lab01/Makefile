server:
	gcc ./servidor.c -o ./out/s_bingus
	./out/s_bingus

client:
	gcc ./cliente.c -o ./out/c_bingus
	./out/c_bingus

stop:
	pkill s_bingus
	pkill c_bingus
	rm -f ./server_socket

rebuild: stop server
