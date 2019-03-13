uart.setup(0, 115200, 8, uart.PARITY_NONE, uart.STOPBITS_1, 1)
-- when 4 chars is received.



uart.on("data", 4,
  function(data)
    print("receive from uart:", data)
    if data=="quit" then
      uart.on("data") -- unregister callback function
    end
end, 0)