// Copyright 2016-2020 ?????????????. All Rights Reserved.
#pragma once
#ifndef STATEMENTS_HOLDER_H
#define STATEMENTS_HOLDER_H

#include <Rendering\Private\ShaderCompiler\Statement.h>

namespace Engine
{
	namespace Rendering
	{
		namespace Private
		{
			namespace ShaderCompiler
			{
				class StatementsHolder
				{
				public:
					StatementsHolder(void)
					{
					}

					const StatementList &GetStatements(void) const
					{
						return m_Statements;
					}

					void AddStatement(Statement *Statement)
					{
						m_Statements.Add(Statement);
					}

				private:
					StatementList m_Statements;
				};
			}
		}
	}
}

#endif