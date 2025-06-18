default: all
.PHONY: default

all:
	@$(MAKE) --no-print-directory -C build
.PHONY: all

run:
	@$(MAKE) --no-print-directory -C build && ./build/grpc-demo
.PHONY: run

bootstrap:
	@chmod +x ./scripts/bootstrap.sh && \
		./scripts/bootstrap.sh
.PHONY: bootstrap

proto:
	@chmod +x ./scripts/proto-gen.sh && \
		./scripts/proto-gen.sh
.PHONY: proto
