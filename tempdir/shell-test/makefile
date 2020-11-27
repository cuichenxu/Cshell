cc = gcc
prom = run
deps = $(shell find ./ -name "*.h*")
src = $(shell find ./ -name "*.c")
obj = $(src:%.c=%.o)

$(prom): $(obj)
	$(cc) -o $(prom) $(obj) -lreadline
%.o: %.c $(deps)
	$(cc) -c $< -o $@ -lreadline
clean:
	rm -rf $(obj)