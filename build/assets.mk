NANAKA_ASSETS_DIR = $(SOURCE_ROOT)/nanaka/assets

assets:
ifneq ($(SOURCE_ROOT),$(findstring $(SOURCE_ROOT),$(ASSETS_OUT_DIR)))
	$(error ASSETS_OUT_DIR not subdir of SOURCE_ROOT)
endif
	rm -rf $(ASSETS_OUT_DIR)
	mkdir -p $(ASSETS_OUT_DIR)/nanaka
	cp -R $(ASSETS_DIR)/* $(ASSETS_OUT_DIR)
	cp -R $(NANAKA_ASSETS_DIR)/* $(ASSETS_OUT_DIR)/nanaka
