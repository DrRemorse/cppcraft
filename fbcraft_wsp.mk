.PHONY: clean All

All:
	@echo "----------Building project:[ fbcraft - Debug ]----------"
	@$(MAKE) -f  "fbcraft.mk"
clean:
	@echo "----------Cleaning project:[ fbcraft - Debug ]----------"
	@$(MAKE) -f  "fbcraft.mk" clean
