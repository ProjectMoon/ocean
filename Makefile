# How to set up remote login information:
# 
# You need to make a file called user.in in the same directory as this
# Makefile. The first line of the file should have your jotunn username
# and the second line should have the path you wish to place files at
# on jotunn.
# 

# remote settings
REMOTE_USER=$(shell cat user.in | sed -n 1p)
REMOTE_MACHINE=jotunn.rhi.hi.is
REMOTE_PATH=$(shell cat user.in | sed -n 2p)

REMOTE_LOGIN=$(REMOTE_USER)@$(REMOTE_MACHINE)
REMOTE_RSYNC_TARGET=$(REMOTE_LOGIN):$(REMOTE_PATH)



# run command: run program remotely from local terminal.
RUN_CMD="mpirun -np 2 $(BINARY)"

# Local rules: rules that are supposed to be run from the development
# machine. They mostly trigger calls to remote rules via ssh.
# Default target: regular remote build.
all: sync build

user:
	@test -s ./user.in || { echo "user.in must exist and have username to do remote commands."; exit 1; }

sync: user
	@printf "\n:: Syncing to remote.\n"
	rsync -avq --delete . $(REMOTE_RSYNC_TARGET)

build: sync
	@printf "\n:: Initiating remote build.\n"
	ssh $(REMOTE_LOGIN) "cd $(REMOTE_PATH) && make local-build"

clean: sync
	@printf "\n:: Remote clean.\n"
	ssh $(REMOTE_LOGIN) "cd $(REMOTE_PATH) && make local-clean"

rebuild: sync
	@printf "\n:: Remote rebuild (clean + build).\n"
	ssh $(REMOTE_LOGIN) "cd $(REMOTE_PATH) && make local-rebuild"

purify:
	@printf "\n:: Cleanse local directory of unworthy files.\n"
	rm -rf *.c~ *.h~ Makefile~

# Other useful commands, meant to be run on the remote system
# Cleans the remote directory
local-clean:
	rm -rf bin/; cd src/ && rm -f *.o

# Runs a build.
local-build:
	make -C src/ all

# Runs clean and then build.
local-rebuild: local-clean local-build
