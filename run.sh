#!/bin/sh

gnome-terminal -- ./bin/server

sleep 2

gnome-terminal -- ./bin/app user1 127.0.0.1 4000

sleep 2

gnome-terminal -- ./bin/app user2 127.0.0.1 4000

sleep 2

gnome-terminal -- ./bin/app user3 127.0.0.1 4000

sleep 2

gnome-terminal -- ./bin/app user4 127.0.0.1 4000