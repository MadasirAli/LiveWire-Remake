#pragma once

#include <string>

#include "Win32.h"

namespace BaghdadCore
{
	class Window
	{
	private:
		class Class final
		{
		public:

			static const Class& Initialize();
			~Class() noexcept;

		private:
			Class();
			Class(const Class& instance) = delete;
			Class(Class&& instance) noexcept = delete;
			Class& operator= (const Class& instance) = delete;
			Class& operator= (Class&& instance) = delete;
			
		public:
			const std::string Name = "BaghdadWindow";
		};
	public:
		bool PeekAndDispatchMessage(MSG* pMsg) const noexcept;
		HWND GetHwnd() const noexcept;

		Window();
		virtual ~Window() noexcept = default;

	protected:
		LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		void InternalCreate(const std::string& name, RECT rect);
		friend LRESULT CALLBACK
			InitWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		friend LRESULT CALLBACK
			StubWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	private:
		HWND _hwnd;
	};
}

