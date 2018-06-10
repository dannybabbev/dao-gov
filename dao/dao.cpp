#include <dao.hpp>
#include <string>

void simpledao::addshrs (account_name payer, account_name to, uint64_t q) {
  accounts accs_t(_self, _self);

  auto toitr = accs_t.find( to );
  if( toitr == accs_t.end() ) {
    accs_t.emplace( payer, [&]( auto& a ) {
      a.owner = to;
      a.shares = q;
    });
  } else {
    accs_t.modify( toitr, 0, [&]( auto& a ) {
      a.shares += q;
      eosio_assert( a.shares >= q, "overflow detected" );
    });
  }
}

void simpledao::createprop(account_name from, std::string prop) {
  require_auth(from);

  proposals prop_t(_self, _self);
  prop_t.emplace(from, [&]( auto& a ){
    a.id = prop_t.available_primary_key();
    a.from = from;
    a.prop = prop;
  });
}


void simpledao::transfer(account_name from, account_name to, uint64_t quantity ) {
  require_auth( from );

  accounts accs_t(_self, _self);

  const auto& fromacnt = accs_t.get( from );
  eosio_assert( fromacnt.shares >= quantity, "overdrawn" );
  accs_t.modify( fromacnt, from, [&]( auto& a ){ a.shares -= quantity; } );

  addshrs( from, to, quantity );
}

void simpledao::issue(account_name to, uint64_t quantity){
  require_auth( _self );
  addshrs( _self, to, quantity );
}

void simpledao::deposit(const account_name from, const asset& quantity) {
  //eosio_assert( quantity.symbol == CORE_SYMBOL, "only core token allowed" );
  eosio_assert( quantity.is_valid(), "invalid bet" );
  eosio_assert( quantity.amount > 0, "must bet positive quantity" );

  action(
    permission_level{from, N(active)},
    N(eosio.token), N(transfer),
    std::make_tuple(from, _self, quantity, std::string(""))
  ).send();
}

void simpledao::createpropf(account_name from, asset& funds, std::string proposal) {
  require_auth(from);

  proposals prop_t(_self, _self);

  prop_t.emplace(from, [&]( auto& a ){
    a.id = prop_t.available_primary_key();
    a.from = from;
    a.prop = proposal;
    a.funds = funds;
  });
}
