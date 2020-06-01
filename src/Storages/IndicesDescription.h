#pragma once

#include <Core/Types.h>

#include <memory>
#include <vector>
#include <Core/Field.h>
#include <Interpreters/ExpressionActions.h>
#include <Parsers/IAST_fwd.h>
#include <Storages/ColumnsDescription.h>

namespace DB
{

/// Description of non-primary index for Storage
struct IndexDescription
{
    /// Definition AST of index
    ASTPtr definition_ast;

    /// List of expressions for index calculation
    ASTPtr expression_list_ast;

    /// Index name
    String name;

    /// Index type (minmax, set, bloom filter, etc.)
    String type;

    /// Prepared expressions for index calculations
    ExpressionActionsPtr expression;

    /// Index arguments, for example probability for bloom filter
    FieldVector arguments;

    /// Names of index columns (not to be confused with required columns)
    Names column_names;

    /// Data types of index columns
    DataTypes data_types;

    /// Sample block with index columns. (NOTE: columns in block are empty, but
    /// not nullptr)
    Block sample_block;

    /// Index granularity, make sense for skip indices
    size_t granularity;

    /// Parse index from definition AST
    static IndexDescription getSkipIndexFromAST(const ASTPtr & definition_ast, const ColumnsDescription & columns, const Context & context);
};

/// All non-primary indicies for storage
struct IndicesDescription : public std::vector<IndexDescription>
{
    /// Index with name exists
    bool has(const String & name) const;
    /// Convert description to string
    String toString() const;
    /// Parse description from string
    static IndicesDescription parse(const String & str, const ColumnsDescription & columns, const Context & context);
};

}
