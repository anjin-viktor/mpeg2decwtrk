mpeg2decwatrk
=============

MPEG-2 Video watermarking on decryption stage

##COMPILING

Requements:

1) cmake

2) boost


	cmake ./ -DCMAKE_BUILD_TYPE=Release
	make

Use `make test` for testing.

##USAGE

1) Create information about positions for watermark embedding

	./build/mpeg2wtrkinfo  -i ./test_info/fair.mpeg -o wtrk_info

2) Create decryption keys for users
	./build/keycreator  --wtrk_info wtrk_info  --users_info ./test_info/users_info  --key ./test_info/key  -o ./out_dir

3) Encryption

	./build/cipher -i test_info/fair.mpeg -o fair.enc -k ../test_info/key

4) Decryption
	./build/cipher -i fair.enc  -o fair1.mpeg -k users_keys/1 
	./build/cipher -i fair.enc  -o fair2.mpeg -k users_keys/2
	./build/cipher -i fair.enc  -o fair3.mpeg -k users_keys/3 
