#!/usr/bin/env sh

ps -ef|grep ./server|awk '{ print $2}'|xargs kill -9