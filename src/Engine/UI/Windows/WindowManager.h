/***************************************************************************************************
* Copyright (c) 2008 Jonathan 'Bladezor' Bastnagel.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the GNU Lesser Public License v2.1
* which accompanies this distribution, and is available at
* http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
* 
* Contributors:
*     Jonathan 'Bladezor' Bastnagel - initial implementation and documentation
***************************************************************************************************/
#ifndef WINDOWMANAGER_H_
#define WINDOWMANAGER_H_

#include "Engine/Base.h"
#include "Window.h"
#include "Engine/UI/Fonts/FontManager.h"
#include "Engine/Input/Input.h"

#include <vector>
#include <iostream>

namespace OE
{
	namespace UI
	{
		namespace Windows
		{
			class WindowManager : public OE::Input::InputListener
			{
			public:
				WindowManager()
				{
					_iWinWidth = 0;
					_iWinHeight = 0;
					_winCurrentFocused = NULL;
					OE::Input::InputManager::AddInputListener(this);
				}

				~WindowManager()
				{
				}

				void Initialize()
				{
					OE::Base::GetWindowSize(&_iWinWidth, &_iWinHeight);
				}

				void OnMouseButton(const int key, const int action);
				void OnMouseClick(const int startX, const int startY, const int endX, const int endY);
				void OnMouseMove(const double x, const double y);
				void OnKeyEvent(const int key, const int action, const int mods);
				void OnCharEvent(const int codepoint);

				void AddWindow(Window &window)
				{
					_vecWindows.push_back(&window);
					SetFocus(static_cast<unsigned int>(_vecWindows.size()-1));
					_winCurrentFocused = &window;
					_winCurrentFocused->Initialize();
				}

				void RemoveWindow(const unsigned int index)
				{
					_vecWindows.at(index)->_bHasFocus = false;
					_vecWindows.erase(_vecWindows.begin() + index);
				}

				bool CanPick(const unsigned int index, const unsigned int x, const unsigned int y);

				void SetFocus(const unsigned int index)
				{
					if(_winCurrentFocused != NULL)
						_winCurrentFocused->_bHasFocus = false;
					_vecWindows[index]->_bHasFocus = true;
				}

				void Update(const float dt);
				void UpdateWin(const int width, const int height);
				void Render(const float dt);

			private:
				void UpdateFocus()
				{
					for(unsigned int i = 0; i < _vecWindows.size(); ++i)
					{
						Window* tempWin = _vecWindows.at(i);
						_winCurrentFocused = tempWin;
						if(tempWin->_bHasFocus && i!=_vecWindows.size()-1)
						{
							_vecWindows.erase(_vecWindows.begin()+i);
							_vecWindows.push_back(tempWin);
						}
					}
				}

				Window *_winCurrentFocused;
				std::vector<Window*> _vecWindows;
				int _iWinWidth, _iWinHeight;
			};
		}
	}
}

#endif /*WINDOWMANAGER_H_*/
