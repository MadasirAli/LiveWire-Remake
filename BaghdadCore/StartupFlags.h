#pragma once

namespace BaghdadCore
{
	class StartupFlags final
	{
	public:
		bool IsLogging() const noexcept;

		StartupFlags(bool logging);

	private:
		bool _logging;
	};
}

