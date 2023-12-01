.PHONY: test verilator

test:
	sbt test

verilator:
	sbt "runMain board.verilator.VerilogGenerator"
	cd verilog/verilator && verilator --trace --exe --cc sim_main.cpp Top.v && make -C obj_dir -f VTop.mk

indent:
	find . -name '*.scala' | xargs scalafmt
	clang-format -i verilog/verilator/*.cpp
	clang-format -i csrc/*.[ch]

clean:
	sbt clean
	-$(MAKE) -C csrc clean

docker-launch-new:
	cp -r ~/.local/share/riscv-none-elf-gcc .
	docker build -t ca-lab3 .
	docker run -d \
	-it \
	--name ca-lab3 \
	--mount type=bind,source="$$(pwd)",target=/app \
	ca-lab3

docker-launch-saved:
	docker start ca-lab3

docker-stop:
	docker stop ca-lab3

docker-attach:
	docker exec -it ca-lab3 /bin/bash
