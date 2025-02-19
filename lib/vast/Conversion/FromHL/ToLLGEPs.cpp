// Copyright (c) 2021-present, Trail of Bits, Inc.

#include "vast/Dialect/HighLevel/Passes.hpp"

VAST_RELAX_WARNINGS
#include <mlir/Analysis/DataLayoutAnalysis.h>
#include <mlir/IR/PatternMatch.h>
#include <mlir/Transforms/GreedyPatternRewriteDriver.h>
#include <mlir/Transforms/DialectConversion.h>
VAST_UNRELAX_WARNINGS

#include "PassesDetails.hpp"

#include "vast/Dialect/HighLevel/HighLevelOps.hpp"
#include "vast/Dialect/LowLevel/LowLevelOps.hpp"

#include "vast/Util/Symbols.hpp"
#include "vast/Util/DialectConversion.hpp"

namespace vast
{
    namespace pattern
    {
        template< typename T >
        struct DoConversion {};

        template<>
        struct DoConversion< hl::RecordMemberOp > : util::State< hl::RecordMemberOp >
        {
            using util::State< hl::RecordMemberOp >::State;

            std::optional< hl::StructDeclOp > get_def(auto op, hl::RecordType named_type) const
            {
                std::optional< hl::StructDeclOp > out;
                auto yield = [&](auto candidate)
                {
                    auto op = candidate.getOperation();
                    if (auto struct_decl = mlir::dyn_cast< hl::StructDeclOp >(op))
                        if (struct_decl.getName() == named_type.getName())
                        {
                            VAST_ASSERT(!out);
                            out = struct_decl;
                        }
                };

                auto tu = op->template getParentOfType< mlir::ModuleOp >();
                if (!tu)
                    return {};

                util::symbols(tu, yield);
                return out;
            }

            std::optional< std::size_t > get_idx(auto name, hl::StructDeclOp decl) const
            {
                std::size_t idx = 0;
                for (auto &maybe_field : solo_block(decl.getFields()))
                {
                    auto field = mlir::dyn_cast< hl::FieldDeclOp >(maybe_field);
                    if (!field)
                        return {};
                    if (field.getName() == name)
                        return { idx };
                    ++idx;
                }
                return {};
            }

            hl::RecordType fetch_record_type(mlir::Type type)
            {
                // TODO(lukas): Rework if we need to handle more cases, probably use
                //              type switch.
                if (auto x = type.dyn_cast< hl::LValueType >())
                    return fetch_record_type(x.getElementType());
                if (auto x = type.dyn_cast< hl::PointerType >())
                    return fetch_record_type(x.getElementType());

                // This is just sugar.
                if (auto elaborated = type.dyn_cast< hl::ElaboratedType >())
                    return fetch_record_type(elaborated.getElementType());

                return type.dyn_cast< hl::RecordType >();

            }

            mlir::LogicalResult convert()
            {
                auto parent_type = operands.getRecord().getType();

                auto as_named_type = fetch_record_type(parent_type);
                if (!as_named_type)
                    return mlir::failure();

                auto maybe_def = get_def(op, as_named_type);
                if (!maybe_def)
                    return mlir::failure();

                auto raw_idx = get_idx(op.getName(), *maybe_def);
                if (!raw_idx)
                    return mlir::failure();

                auto gep = rewriter.create< ll::StructGEPOp >(
                        op.getLoc(),
                        op.getType(),
                        operands.getRecord(),
                        rewriter.getI32IntegerAttr(*raw_idx),
                        op.getNameAttr());
                rewriter.replaceOp( op, { gep } );

                return mlir::success();
            }

        };

        using record_member_op = util::BasePattern< hl::RecordMemberOp, DoConversion >;

    } // namespace pattern

    struct HLToLLGEPsPass : HLToLLGEPsBase< HLToLLGEPsPass >
    {
        void runOnOperation() override
        {
            auto op = this->getOperation();
            auto &mctx = this->getContext();

            mlir::ConversionTarget trg(mctx);
            trg.markUnknownOpDynamicallyLegal( [](auto) { return true; } );
            trg.addIllegalOp< hl::RecordMemberOp >();

            mlir::RewritePatternSet patterns(&mctx);

            patterns.add< pattern::record_member_op >(&mctx);

            if (mlir::failed(mlir::applyPartialConversion(op, trg, std::move(patterns))))
                return signalPassFailure();
        }
    };
} // namespace vast


std::unique_ptr< mlir::Pass > vast::createHLToLLGEPsPass()
{
    return std::make_unique< vast::HLToLLGEPsPass >();
}
