CFLAGS=-O3 -lm

%: %.c %.kari ../ikari
	$(CC) $(CFLAGS) -o $@ $<
	@echo "*** Running benchmark for $<..."
	@time -p ./$@
	@rm $@
	@echo "*** Running benchmark for $@.kari..."
	@time -p ../ikari -x < $@.kari
	@echo ""
	@true