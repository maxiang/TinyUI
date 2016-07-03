#pragma once
#include "TinyVisual.h"

namespace TinyUI
{
	namespace Windowless
	{
		/// <summary>
		/// �����������ؼ�
		/// </summary>
		class TinyVisualLabel : public TinyVisual
		{
			friend class TinyVisualFactory;
			DECLARE_DYNAMIC(TinyVisualLabel);
			DISALLOW_COPY_AND_ASSIGN(TinyVisualLabel);
		protected:
			TinyVisualLabel(TinyVisual* spvisParent, TinyVisualTree* vtree);
		public:
			virtual ~TinyVisualLabel();
			TinyString RetrieveTag() const OVERRIDE;
			BOOL OnDraw(HDC hDC) OVERRIDE;
		};
	}
}


