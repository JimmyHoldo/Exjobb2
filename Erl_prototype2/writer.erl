-module(writer).

-export([start/2]).

start(Arg, ExtPrg) ->
  io:format("supervisor started writer! Arg=~s~n", [Arg]),
  Pid = spawn_link(fun() -> init(Arg, ExtPrg) end),
  register(writerloop, Pid),
  {ok, Pid}.

init(Arg, ExtPrg) ->
	PortWriter = open_port({spawn_executable, ExtPrg}, [{packet, 2}]),
	Serial_fd = serialport:open_port_serial(PortWriter, Arg),
	serialport:initport(PortWriter, Serial_fd),
	loop(PortWriter, Serial_fd).

loop(PortWriter, Serial_fd) ->
  receive
		{msgToWriter, Msg} ->
            io:format("~s~n", [Msg]),
			serialport:write_to_zigbee(PortWriter, [Serial_fd, Msg, 0])
  end,
  loop(PortWriter, Serial_fd).

% c(writer). c(serialport). writer:start("/dev/ttyUSB2", "readerwriterprg").
