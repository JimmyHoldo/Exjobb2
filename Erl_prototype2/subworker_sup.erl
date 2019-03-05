-module(subworker_sup).
-behaviour(supervisor).

-export([start_link/3]).
-export([init/1]).

start_link(Arg, ExtPrg, Time) ->
  supervisor:start_link({local,?MODULE}, ?MODULE, [Arg, ExtPrg, Time]).

init([Arg, ExtPrg, Time]) ->
    SupFlags = {one_for_one, 1, 5},
    ChildSpecs = [{reader,
                   {reader, start, [Arg, ExtPrg]},
                   permanent,
                   1000,
                   worker,
                   [reader]},
                  {datagen,
                   {datagen, start, [Time]},
                   permanent,
                   1000,
                   worker,
                   [datagen]}],
    {ok, {SupFlags, ChildSpecs}}.
