//
// Created by luoqiaoyou on 2018/5/25.
//

#include <graphene/app/analysis_api.hpp>

namespace graphene {
    namespace app {

        // analysis_api
        analysis_api::analysis_api(graphene::chain::database& db) : _db(db) { }
        analysis_api::~analysis_api() { }

        vector<account_asset_balance_snapshot> analysis_api::get_asset_holders_snapshot( asset_id_type asset_id, uint32_t limit) const {

           const auto& bal_idx = _db.get_index_type< account_balance_index >().indices().get< by_asset_balance >();
           auto range = bal_idx.equal_range( boost::make_tuple( asset_id ) );

           vector<account_asset_balance_snapshot> result;

           uint32_t index = 0;
           for( const account_balance_object& bal : boost::make_iterator_range( range.first, range.second ) )
           {
              if (index++ > limit && limit != 0) {
                 break;
              }

              const auto account = _db.find(bal.owner);

              account_asset_balance_snapshot aab;
              aab.name       = account->name;
              aab.id = account->id;
              aab.m     = bal.balance.value;


              // Add the account's limit orders
              auto order_range = _db.get_index_type<limit_order_index>().indices().get<by_account>().equal_range(account->id);
              std::for_each(order_range.first, order_range.second,
                            [&aab, &asset_id] (const limit_order_object& order) {
                               if (order.sell_price.base.asset_id == asset_id)
                                  aab.om += order.for_sale;
                            });
              // Add the account's call orders
              auto call_range = _db.get_index_type<call_order_index>().indices().get<by_account>().equal_range(account->id);
              std::for_each(call_range.first, call_range.second,
                            [&aab, &asset_id] (const call_order_object& call) {
                               if (call.call_price.quote.asset_id == asset_id) {
                                  aab.dm += call.debt;
                                  aab.cm += call.collateral;
                               }
                            });


              result.push_back(aab);
           }

           return result;
        }

        // get number of asset holders.
        int analysis_api::get_asset_holders_count( asset_id_type asset_id ) const {

           const auto& bal_idx = _db.get_index_type< account_balance_index >().indices().get< by_asset_balance >();
           auto range = bal_idx.equal_range( boost::make_tuple( asset_id ) );

           int count = boost::distance(range) - 1;

           return count;
        }
    }}