args = arg(1, 'a')
sum = arg()

do i = 1 to arg()
   arg = args[i]
   if arg \= .nil then
      sum += arg
end

return sum
