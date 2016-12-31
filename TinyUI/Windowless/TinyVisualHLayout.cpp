#include "../stdafx.h"
#include "TinyVisualScrollbar.h"
#include "TinyVisualDocument.h"
#include "TinyVisualHLayout.h"

namespace TinyUI
{
	namespace Windowless
	{
		TinyVisualHLayout::TinyVisualHLayout(TinyVisual* spvisParent, TinyVisualDocument* vtree)
			:TinyVisual(spvisParent, vtree),
			m_bAuto(FALSE)
		{

		}
		TinyVisualHLayout::~TinyVisualHLayout()
		{

		}
		TinyString TinyVisualHLayout::RetrieveTag() const
		{
			return TinyVisualTag::HORIZONTALLAYOUT;
		}
		BOOL TinyVisualHLayout::IsLayout() const
		{
			return TRUE;
		}

		void TinyVisualHLayout::OnSizeChange(const TinySize& size)
		{
			TinyArray<TinyVisual*> array;
			TinyVisual* spvis = m_document->GetVisual(this, CMD_CHILD);
			while (spvis)
			{
				array.Add(spvis);
				spvis = m_document->GetVisual(spvis, CMD_NEXT);
			}
			if (array.GetSize() > 0)
			{
				INT ave = TO_CX(m_rectangle) / array.GetSize();
				switch (m_horizontalAlignment)
				{
				case TinyUI::Windowless::HORIZONTAL_LEFT:
				{
					INT x = 0;
					for (INT i = 0;i < array.GetSize();i++)
					{
						spvis = array[i];
						INT y = 0;
						if (!spvis->Contain(TinyVisualProperty::SIZE))
						{
							spvis->SetSize(TinySize(ave, TO_CY(m_rectangle)));
						}
						switch (m_verticalAlignment)
						{
						case TinyUI::Windowless::VERTICAL_CENTER:
							y = (TO_CY(m_rectangle) - spvis->GetSize().cy) / 2;
							break;
						case TinyUI::Windowless::VERTICAL_BOTTOM:
							y = TO_CY(m_rectangle) - spvis->GetSize().cy;
							break;
						case TinyUI::Windowless::VERTICAL_TOP:
						case TinyUI::Windowless::VERTICAL_STRETCH:
						default:
							y = 0;
							break;
						}
						spvis->SetPosition(TinyPoint(x, y));
						x += spvis->GetSize().cx;

					}
				}
				break;
				case TinyUI::Windowless::HORIZONTAL_CENTER:
				{

				}
				break;
				case TinyUI::Windowless::HORIZONTAL_RIGHT:
				{
					INT x = TO_CX(m_rectangle);
					for (INT i = 0;i < array.GetSize();i++)
					{
						spvis = array[i];
						INT y = 0;
						if (!spvis->Contain(TinyVisualProperty::SIZE))
						{
							spvis->SetSize(TinySize(ave, TO_CY(m_rectangle)));
						}
						switch (m_verticalAlignment)
						{
						case TinyUI::Windowless::VERTICAL_CENTER:
							y = (TO_CY(m_rectangle) - spvis->GetSize().cy) / 2;
							break;
						case TinyUI::Windowless::VERTICAL_BOTTOM:
							y = TO_CY(m_rectangle) - spvis->GetSize().cy;
							break;
						case TinyUI::Windowless::VERTICAL_TOP:
						case TinyUI::Windowless::VERTICAL_STRETCH:
						default:
							y = 0;
							break;
						}
						x -= spvis->GetSize().cx;
						spvis->SetPosition(TinyPoint(x, y));
						TRACE("spvis:%s,size-cx:%d,size-cy:%d,pos-x:%d,pos-y:%d\n", spvis->GetName().STR(), spvis->GetSize().cx, spvis->GetSize().cy, spvis->GetPosition().x, spvis->GetPosition().y);
					}
				}
				break;
				case TinyUI::Windowless::HORIZONTAL_STRETCH://SIZE��POS��Ч
				default:
				{
					INT x = 0;
					for (INT i = array.GetSize() - 1;i >= 0;i--)
					{
						spvis = array[i];
						INT y = 0;
						if (!spvis->Contain(TinyVisualProperty::SIZE))
						{
							spvis->SetSize(TinySize(ave, TO_CY(m_rectangle)));
						}
						switch (m_verticalAlignment)
						{
						case TinyUI::Windowless::VERTICAL_CENTER:
							y = (TO_CY(m_rectangle) - spvis->GetSize().cy) / 2;
							break;
						case TinyUI::Windowless::VERTICAL_BOTTOM:
							y = TO_CY(m_rectangle) - spvis->GetSize().cy;
							break;
						case TinyUI::Windowless::VERTICAL_TOP:
						case TinyUI::Windowless::VERTICAL_STRETCH:
						default:
							y = 0;
							break;
						}
						spvis->SetPosition(TinyPoint(x, y));
						x += spvis->GetSize().cx;
					}
				}
				break;
				}
			}
		}

		void TinyVisualHLayout::AutoScroll(BOOL bAuto)
		{
			m_bAuto = bAuto;
		}
		void TinyVisualHLayout::SetHorizontalAlignment(HorizontalAlignment horizontalAlignment)
		{
			m_horizontalAlignment = horizontalAlignment;
		}
		void TinyVisualHLayout::SetVerticalAlignment(VerticalAlignment verticalAlignment)
		{
			m_verticalAlignment = verticalAlignment;
		}
		HRESULT	TinyVisualHLayout::OnCreate()
		{
			ASSERT(m_document);
			return TinyVisual::OnCreate();
		}
		HRESULT TinyVisualHLayout::OnDestory()
		{
			return TinyVisual::OnDestory();
		}
		BOOL TinyVisualHLayout::OnDraw(HDC hDC, const RECT& rcPaint)
		{
			return TRUE;
		}
		HRESULT	TinyVisualHLayout::OnMouseEnter()
		{
			return FALSE;
		}
		HRESULT	TinyVisualHLayout::OnMouseLeave()
		{
			return FALSE;
		}
	}
}
