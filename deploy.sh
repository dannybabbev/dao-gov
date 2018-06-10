#!/bin/bash

alias cleos='docker exec eosio /opt/eosio/bin/cleos --wallet-url http://localhost:8888'
alias eosiocpp='docker exec eosio eosiocpp'

docker cp dao/. eosio:/dao && eosiocpp -o /dao/dao.wast /dao/dao.cpp && eosiocpp -g /dao/dao.abi /dao/dao.hpp

cleos wallet unlock --password PW5JgnbofGVA3UoMRVM946DTR4wVya5WcJTtTeD1LqwJZGwf45hcq
cleos set contract dao /dao -p dao

cleos push action dao issue '[ "dao", 1000]' -p dao
cleos push action dao transfer '["dao", "daniel", 100]' -p dao
cleos push action dao createprop '["dao", "This is my proposal"]' -p dao
cleos push action dao createpropf '["dao", "100.0000 SYS" ,"This is my proposal"]' -p dao


docker cp simple.token/. eosio:/simple.token && eosiocpp -o /simple.token/simple.token.wast /simple.token/simple.token.cpp && eosiocpp -g /simple.token/simple.token.abi /simple.token/simple.token.cpp

cleos push action eosio updateauth '{"account":"user","permission":"active","parent":"owner","auth":{"keys":[{"key":"EOS7ckzf4BMgxjgNSYV22rtTXga8R9Z4XWVhYp8TBgnBi2cErJ2hn", "weight":1}],"threshold":1,"accounts":[{"permission":{"actor":"contract","permission":"eosio.code"},"weight":1}],"waits":[]}}' -p user
