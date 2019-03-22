-module(app).
-behaviour(application).

-export([start/2]).

start(normal, Args) ->
  %io:format("supervisor started writer! Arg=~s~n", [Args]),
  worker_sup:start(Args).

stop(_State) ->
    ok.

% c(writer). c(serialport). writer:start("/dev/ttyUSB2", "readerwriterprg").
