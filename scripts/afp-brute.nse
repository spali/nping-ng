description = [[
Performs password guessing against Apple Filing Protocol (AFP)
]]

---
-- @usage 
-- nmap -p 548 --script afp-brute <host>
--
-- @output
-- PORT    STATE SERVICE
-- 548/tcp open  afp
-- | afp-brute:  
-- |_  admin:KenSentMe => Login Correct
--
--
-- Information on AFP implementations
--
-- Snow Leopard
-- ------------
-- - Delay 10 seconds for accounts with more than 5 incorrect login attempts (good)
-- - Instant response if password is successfull
--
-- Netatalk
-- --------
-- - Netatalk responds with a "Parameter error" when the username is invalid
--

author = "Patrik Karlsson"
license = "Same as Nmap--See http://nmap.org/book/man-legal.html"
categories = {"intrusive", "auth"}

require 'shortport'
require 'stdnse'
require 'afp'
require 'unpwdb'

-- Version 0.2
-- Created 01/15/2010 - v0.1 - created by Patrik Karlsson <patrik@cqure.net>
-- Revised 03/09/2010 - v0.2 - changed so that passwords are iterated over users
--                           - this change makes better sence as guessing is slow

portrule = shortport.port_or_service(548, "afp")

action = function( host, port )

	local max_time = unpwdb.timelimit() ~= nil and unpwdb.timelimit() * 1000 or -1
	local clock_start = nmap.clock_ms()
	local result, response, status, aborted = {}, nil, nil, false	
	local valid_accounts, found_users = {}, {}
	local helper
	
 	status, usernames = unpwdb.usernames()
	if not status then return end

	status, passwords = unpwdb.passwords()	
	if not status then return end
	
	for password in passwords do
		for username in usernames do
			if ( not(found_users[username]) ) then
				if max_time>0 and nmap.clock_ms() - clock_start >  max_time then
					aborted=true
					break
				end

				helper = afp.Helper:new()
				status, response = helper:OpenSession( host, port )

				if ( not(status) ) then
					stdnse.print_debug("OpenSession failed")
					return
				end


				stdnse.print_debug( string.format("Trying %s/%s ...", username, password ) )
				status, response = helper:Login( username, password )

				-- if the response is "Parameter error." we're dealing with Netatalk
				-- This basically means that the user account does not exist
				-- In this case, why bother continuing? Simply abort and thank Netatalk for the fish
				if response:match("Parameter error.") then
					stdnse.print_debug("Netatalk told us the user does not exist! Thanks.")
					-- mark it as "found" to skip it
					found_users[username] = true
				end

				if status then				
					-- Add credentials for other afp scripts to use
					if nmap.registry.afp == nil then
						nmap.registry.afp = {}
					end	
					nmap.registry.afp[username]=password
					found_users[username] = true

					table.insert( valid_accounts, string.format("%s:%s => Login Correct", username, password:len()>0 and password or "<empty>" ) )
					break
				end
				helper:CloseSession()
			end
		end	
		usernames("reset")
	end
	
	local output = stdnse.format_output(true, valid_accounts)

	if max_time > 0 and aborted then
		output = ( output or "" ) .. string.format(" \n\nscript aborted execution after %d seconds", max_time/1000 )
	end
	
	return output
	
end