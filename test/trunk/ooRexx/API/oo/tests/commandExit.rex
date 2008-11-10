use arg address, command
signal on error
signal on failure

address value address
command

return "SUCCESS" rc

error:
return 'ERROR' rc

failure:
return 'FAILURE' rc
