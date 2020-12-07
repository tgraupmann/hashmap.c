test:
	if [ ! -d "./out" ]; then mkdir "./out"; fi
	gcc test1.c ../hashmap.c -o ./out/test1 -pthread -std=c11
	./out/test1
