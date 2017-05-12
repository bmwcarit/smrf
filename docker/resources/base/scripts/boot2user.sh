#!/bin/sh

# Access shared volumes as non root user
#
# Usage: docker run -e DEV_UID=$(id -u) ...
# Source: http://chapeau.freevariable.com/2014/08/docker-uid.html

USER=smrf

export DEV_UID=${DEV_UID:=1000}

useradd -u $DEV_UID -m $USER
chown -R $USER:$USER /home/$USER

cd /home/$USER

if [ $# -eq 0 ]; then
        # no commands supplied
        export HOME=/home/$USER
        exec gosu $DEV_UID /bin/bash
else
        # commands supplied
        export HOME=/home/$USER
        exec gosu $USER /bin/bash -c "source $HOME/.bashrc; $*"
fi
