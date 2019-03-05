-module(serialport).

-export([open_port_serial/2, initport/2,
        read_from_zigbee/2, write_to_zigbee/2, stop/1]).

open_port_serial(Pid, Portname) ->
    decode(call_port(Pid, {open_port_serial, Portname})).

initport(Pid, Portinit) ->
    call_port(Pid, {initport, Portinit}).

read_from_zigbee(Pid, Msg) ->
    call_port(Pid, {read_from_zigbee, Msg}).

write_to_zigbee(Pid, Msg) ->
    call_port(Pid, {write_to_zigbee, Msg}).

call_port(Pid, Msg) ->
    process_flag(trap_exit, true),
    Pid ! {self(), {command, encode(Msg)}},
    receive
        {Pid, {data, Data}} ->
            Data;
        {'EXIT', Port, _} ->
            stop(Port),
            exit(port_terminated)
    end.

stop(Port) ->
    Port ! {self(), close},
    receive
        {Port, closed} ->
            exit(normal)
    end.

encode({initport, X}) -> [1, X];
encode({open_port_serial, Y}) -> [2, Y, 0];
encode({read_from_zigbee, Z}) -> [3, Z];
encode({write_to_zigbee, Msg}) -> [4, Msg].

decode([Int]) -> Int.
