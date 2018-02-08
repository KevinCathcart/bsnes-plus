include bsnes/nall/Makefile

.PHONY: all bsnes dist clean

default: bsnes

bsnes:
	@$(MAKE) -C bsnes deploy

all: clean
	@$(MAKE) -C bsnes deploy
	@$(MAKE) clean -C bsnes
	@$(MAKE) -C bsnes deploy profile=accuracy
	@$(MAKE) clean -C bsnes

dist:
ifeq ($(platform),osx)
	@./build_distribution_osx
endif

clean:
	@$(MAKE) clean -C bsnes
ifeq ($(platform),osx)
	@rm -frd ./bsnes+.app
	@rm -frd ./bsnes+accuracy.app
endif
