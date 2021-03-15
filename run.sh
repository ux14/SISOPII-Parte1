#!/bin/sh

gnome-terminal -- ./bin/servidor

sleep 2

gnome-terminal -- ./bin/cliente 127.0.0.1

sleep 2

gnome-terminal -- ./bin/cliente 127.0.0.1

sleep 2

gnome-terminal -- ./bin/cliente 127.0.0.1

sleep 2

gnome-terminal -- ./bin/cliente 127.0.0.1