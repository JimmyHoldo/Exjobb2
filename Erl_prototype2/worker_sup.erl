-module(worker_sup).
-behaviour(supervisor).

-export([start_link/3]).
-export([init/1]).

start_link(Arg, ExtPrg, Time) ->
  supervisor:start_link({local,?MODULE}, ?MODULE, [Arg, ExtPrg, Time]).

init([Arg, ExtPrg, Time]) ->
    SupFlags = {one_for_all, 1, 5},
    ChildSpecs = [{writer,
                   {writer, start, [Arg, ExtPrg]},
                   permanent,
                   1000,
                   worker,
                   [writer]},
                  {subworker_sup,
                   {subworker_sup, start_link, [Arg, ExtPrg, Time]},
                   permanent,
                   1000,
                   supervisor,
                   [subworker_sup]}],
    {ok, {SupFlags, ChildSpecs}}.


% c(worker_sup). c(subworker_sup). c(writer). c(reader). c(datagen).
% worker_sup:start_link("/dev/ttyUSB1", "readerwriterprg")

% c(worker_sup). c(subworker_sup). c(writer). c(reader). c(datagen). c(serialport). worker_sup:start_link("/dev/ttyUSB0", "readerwriterprg", 1000).
