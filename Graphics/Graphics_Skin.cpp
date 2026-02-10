#include "Graphics.h"

#include <Types/Graphics\Skin.h>

void Graphics::DrawSkin(int x, int y, int width, int height, Skin& skin, double opacity, bool drawLeft, bool drawTop, bool drawRight, bool drawBottom)
{
	double xShrink = 1;
	double yShrink = 1;

	int minWidth = skin.LW + skin.RW;
	int minHeight = skin.TH + skin.BH;

	if (width + skin.OW < minWidth)
		xShrink = (double)(width + skin.OW) / (double)minWidth;

	if (height + skin.OH < minHeight)
		yShrink = (double)(height + skin.OH) / (double)minHeight;

	// Trim the sides

	int leftWidth = drawLeft 
		? skin.LW 
		: -skin.OX; // left flab
	
	int rightWidth = drawRight 
		? skin.RW 
		: skin.OW - -skin.OX; // right flab

	int topHeight = drawTop 
		? skin.TH 
		: -skin.OY; // top flab

	int bottomHeight = drawBottom 
		? skin.BH 
		: skin.OH - -skin.OY; // bottom flab

	// Top Left

	if (drawTop && drawLeft)
	{
		int tlx = skin.OX;
		int tly = skin.OY;
		int tlw = skin.LW;
		int tlh = skin.TH;

		tlx = (int)((double)tlx * xShrink);
		tly = (int)((double)tly * yShrink);
		tlw = (int)((double)tlw * xShrink);
		tlh = (int)((double)tlh * yShrink);

		DrawImage(
			x + tlx, 
			y + tly, 
			tlw, 
			tlh, 
			skin.Image,
			skin.X,	
			skin.Y, 
			skin.LW, 
			skin.TH, 
			opacity);
	}

	// Top

	if (drawTop)
	{
		int tx = skin.OX + leftWidth;
		int ty = skin.OY;
		int tw = max(0, skin.OW + width - leftWidth - rightWidth);
		int th = skin.TH;

		if (xShrink < 1) 
		{
			tx = (int)((double)tx * xShrink);
		}

		if (yShrink < 1)
		{
			ty = (int)((double)ty * yShrink);
			th = (int)((double)th * yShrink);
		}

		DrawImage(
			x + tx,
			y + ty,
			tw,
			th,
			skin.Image,
			skin.X + skin.LW,
			skin.Y,
			skin.Width - skin.LW - skin.RW,
			skin.TH,
			opacity);
	}

	// Top Right

	if (drawTop && drawRight)
	{
		int trx = skin.OX + skin.OW - skin.RW;
		int try_= skin.OY;
		int trw = skin.RW;
		int trh = skin.TH;

		trx = width + (int)((double)trx * xShrink);
		try_= (int)((double)try_* yShrink);
		trw = (int)((double)trw * xShrink);
		trh = (int)((double)trh * yShrink);

		DrawImage(
			x + trx, 
			y + try_, 
			trw,
			trh,
			skin.Image, 
			skin.X + skin.Width - skin.RW,
			skin.Y, 
			skin.RW, 
			skin.TH, 
			opacity);
	}

	// Left

	if (drawLeft)
	{
		int lx = skin.OX;
		int ly = skin.OY + topHeight;
		int lw = skin.LW;
		int lh = max(0, skin.OH + height - topHeight - bottomHeight);

		lx = (int)((double)lx * xShrink);
		ly = (int)((double)ly * yShrink);
		lw = (int)((double)lw * xShrink);

		DrawImage(
			x + lx,
			y + ly,
			lw,
			lh,
			skin.Image,
			skin.X,
			skin.Y + skin.TH,
			skin.LW,
			skin.Height - skin.TH - skin.BH,
			opacity);
	}

	// Center

	int cx = skin.OX + leftWidth;
	int cy = skin.OY + topHeight;
	int cw = max(0, skin.OW + width - leftWidth - rightWidth);
	int ch = max(0, skin.OH + height - topHeight - bottomHeight);

	cx = (int)((double)cx * xShrink);
	cy = (int)((double)cy * yShrink);

	DrawImage(
		x + cx,
		y + cy,
		cw,
		ch,
		skin.Image,
		skin.X + skin.LW,
		skin.Y + skin.TH,
		skin.Width - skin.LW - skin.RW,
		skin.Height - skin.TH - skin.BH,
		opacity);

	// Right

	if (drawRight)
	{
		int rx = skin.OX + skin.OW - rightWidth;
		int ry = skin.OY + topHeight;
		int rw = skin.RW;
		int rh = max(0, skin.OH + height - topHeight - bottomHeight);

		rx = width + (int)((double)rx * xShrink);
		ry = (int)((double)ry * yShrink);
		rw = (int)((double)rw * xShrink);

		DrawImage(
			x + rx, 
			y + ry,
			rw,
			rh,
			skin.Image,
			skin.X + skin.Width - skin.RW,
			skin.Y + skin.TH,
			skin.RW,
			skin.Height - skin.TH - skin.BH,
			opacity);
	}

	// Bottom Left

	if (drawBottom && drawLeft)
	{
		int blx = skin.OX;
		int bly = skin.OY + skin.OH - skin.BH;
		int blw = skin.LW;
		int blh = skin.BH;

		blx = (int)((double)blx * xShrink);
		bly = height + (int)((double)bly * yShrink);
		blw = (int)((double)blw * xShrink);
		blh = (int)((double)blh * yShrink);

		DrawImage(
			x + blx, 
			y + bly, 
			blw, 
			blh, 
			skin.Image, 
			skin.X,	
			skin.Y + skin.Height - skin.BH, 
			skin.LW, 
			skin.BH, 
			opacity);
	}

	// Bottom

	if (drawBottom)
	{
		int bx = skin.OX + leftWidth;
		int by = skin.OY + skin.OH - bottomHeight;
		int bw = max(0, skin.OW + width - leftWidth - rightWidth);
		int bh = skin.BH;

		bx = (int)((double)bx * xShrink);
		by = height + (int)((double)by * yShrink);
		bh = (int)((double)bh * yShrink);

		DrawImage(
			x + bx,
			y + by,
			bw,
			bh,
			skin.Image,
			skin.X + skin.LW,
			skin.Y + skin.Height - skin.BH,
			skin.Width - skin.LW - skin.RW,
			skin.BH,
			opacity);
	}

	// Bottom Right

	if (drawBottom && drawRight)
	{
		int brx = skin.OX + skin.OW - skin.RW;
		int bry = skin.OY + skin.OH - skin.BH;
		int brw = skin.RW;
		int brh = skin.BH;

		brx = width + (int)((double)brx * xShrink);
		bry = height + (int)((double)bry * yShrink);
		brw = (int)((double)brw * xShrink);
		brh = (int)((double)brh * yShrink);

		DrawImage(
			x + brx,
			y + bry,
			brw,
			brh,
			skin.Image, 
			skin.X + skin.Width - skin.RW,	
			skin.Y + skin.Height - skin.BH,
			skin.RW, 
			skin.BH, 
			opacity);
	}
}

void Graphics::DrawSkin(const Rect& rect, Skin& skin, double opacity)
{
	DrawSkin(rect.X, rect.Y, rect.Width, rect.Height, skin, opacity);
}