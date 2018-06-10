#pragma once

#include <eosiolib/eosio.hpp>
#include <string>
#include <eosiolib/asset.hpp>


using namespace eosio;

class simpledao : public contract {
public:
  simpledao( account_name self ) : contract(self){}

  // @abi action
  void transfer(account_name from, account_name to, uint64_t quantity);

  // @abi action
  void issue( account_name to, uint64_t quantity );

  // @abi action
  void addshrs(account_name payer, account_name to, uint64_t q);

  // @abi action
  void createprop(account_name from, std::string prop);

  // @abi action
  void deposit(const account_name from, const asset& funds);

  // @abi action
  void createpropf(account_name from, asset& funds, std::string proposal);

private:
  /// @abi table accounts i64
  struct account {
    account_name owner;
    uint64_t     shares;

    uint64_t primary_key()const { return owner; }

    EOSLIB_SERIALIZE(account, (owner) (shares))
  };

  typedef eosio::multi_index<N(accounts), account> accounts;

  /// @abi table proposals i64
  struct proposal {
    uint64_t     id;
    account_name from;
    std::string  prop;
    asset        funds;

    uint64_t primary_key()const { return id; }

    EOSLIB_SERIALIZE(proposal, (id) (from) (prop) (funds))
  };

  typedef eosio::multi_index<N(proposals), proposal> proposals;
};

EOSIO_ABI(simpledao, (transfer) (issue) (addshrs) (createprop) (deposit) (createpropf))
