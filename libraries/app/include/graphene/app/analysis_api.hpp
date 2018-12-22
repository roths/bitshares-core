//
// Created by luoqiaoyou on 2018/5/25.
//

#pragma once

#include <graphene/app/database_api.hpp>
#include <graphene/chain/protocol/types.hpp>

namespace graphene {
    namespace app {
        using namespace graphene::chain;

       struct account_asset_balance_snapshot
       {
          string          name;
          account_id_type id;
          share_type      m; //amount
          share_type      om; //order_amount
          share_type      dm; //debt_amount
          share_type      cm; //collateral_amount
       };

        class analysis_api {
           public:
            analysis_api(graphene::chain::database &db);

            ~analysis_api();

            vector<account_asset_balance_snapshot> get_asset_holders_snapshot(asset_id_type asset_id, uint32_t limit) const;

            int get_asset_holders_count(asset_id_type asset_id) const;

           private:
            graphene::chain::database &_db;
        };

    }
}

FC_REFLECT( graphene::app::account_asset_balance_snapshot, (name)(id)(m)(om)(dm)(cm) );


FC_API(graphene::app::analysis_api,
       (get_asset_holders_snapshot)
          (get_asset_holders_count)
)

