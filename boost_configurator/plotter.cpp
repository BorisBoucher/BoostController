#include <QPainter>
#include <QtMath>
#include <QMouseEvent>
#include "plotter.h"

Plotter::Plotter(QWidget *parent) : QWidget(parent)
{
	setMouseTracking(true);

	connect(&mTimer, SIGNAL(timeout()), this, SLOT(update()));
//	installEventFilter();

	std::vector<ChannelInfo> channels;
	channels.push_back(ChannelInfo{"RPM", "RMP"});
	channels.push_back(ChannelInfo{"THROTTLE", "%"});
	channels.push_back(ChannelInfo{"BOOST", "Bar"});

	setChannels(channels);

	mTimer.setInterval(30);
	mTimer.start();

}

void Plotter::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setFont(QFont("Courier", 9, 1));

	auto surface = this->rect();

	// Black backgound
	painter.setBrush(QBrush(QColor(0,0,0)));
	painter.drawRect(surface);

	// Time line
	auto solidPen = QPen(QBrush(QColor(128,128,128)), 2);
	painter.setPen(solidPen);
	painter.drawLine(0, surface.bottom() - 16, surface.width(), surface.bottom() - 16);

	// Vertical line
	painter.drawLine(44, 0, 44, surface.bottom() - 16);

	auto dataRect = QRect(44, 0, surface.width() - 44, surface.height() - 16);
	// Data grid
	auto dashPen = QPen(QBrush(QColor(128,128,128)), 1, Qt::DashLine);
	for (size_t i = 1; i<10; ++i)
	{
		if (i != 5)
			painter.setPen(dashPen);
		else
			painter.setPen(solidPen);
		painter.drawLine(dataRect.left(), dataRect.bottom() / 10.0 * i,
						 dataRect.right(), dataRect.bottom() / 10.0 * i);
		painter.drawLine(dataRect.left() + dataRect.width() / 10.0 * i, dataRect.top(),
						 dataRect.left() + dataRect.width() / 10.0 * i, dataRect.bottom());
	}

	// Time span
	painter.setPen(QPen(QColor(64,64,64), 1));
	painter.setBrush(QBrush(QColor(64,64,64)));
	QRect span(dataRect.left() + 80, dataRect.bottom() + 4, 200, 16 - 8);
	painter.drawRect(span);
	painter.setPen(QPen(QColor(128,128,128), 1));
	painter.drawLine(span.topLeft(), span.bottomLeft());
	painter.drawLine(span.bottomLeft(), QPoint(span.left() + 4, span.top() + 4));
	painter.drawLine(QPoint(span.left() + 4, span.top() + 4), span.topLeft());
	painter.drawLine(span.topRight(), span.bottomRight());
	painter.drawLine(span.bottomRight(), QPoint(span.right() - 4, span.top() + 4));
	painter.drawLine(QPoint(span.right() - 4, span.top() + 4), span.topRight());

	// Time values
	painter.setPen(QPen(QColor(200,200,200), 1));
	for (size_t i=0; i<10; ++i)
	{
		int x = dataRect.left() + dataRect.width() / 10.0 * i;
		double t = mCurrentLeftTime + i * mTimeDiv;
		painter.drawText(x-20, dataRect.bottom(), 40, 16, Qt::AlignCenter| Qt::AlignTop, QString::number(t));
	}

	// Y values
	auto chan1Pen = QPen(QPen(QColor(255,80,80), 2));
	auto chan2Pen = QPen(QPen(QColor(80,255,80), 2));
	painter.setPen(chan1Pen);
	painter.drawText(dataRect.left() + 2, 0, 100, 16, Qt::AlignLeft | Qt::AlignTop, "VALUE1");
	painter.drawText(0, 0, 42, 16, Qt::AlignRight | Qt::AlignTop, "1234");
	painter.drawText(0, dataRect.bottom()-16, 42,16, Qt::AlignRight | Qt::AlignBottom, "0");

	// Some random value
	size_t nbS = dataRect.width();
//	std::vector<History::Sample> samples(dataRect.width() / 2 * 3);
	std::vector<History::Sample> samples(nbS);
	mHistories.at(0).getSamples(mCurrentLeftTime, mCurrentLeftTime + 10.0 * mTimeDiv, samples.data(), samples.size());
	std::vector<QPoint> points;
	points.reserve(dataRect.width());
	for (int i=0; i<dataRect.width(); ++i)
	{
//		double t = double(i) / dataRect.width() * 2 * M_PI * 5.0 * mTimeDiv;
//		double t = mCurrentLeftTime + double(i) / dataRect.width() * 10.0 * mTimeDiv;
//		points.push_back(QPoint(dataRect.left() + i, dataRect.height() / 2.0 + dataRect.height() * sin(t) / 2.0 ));
		if (!isnan(samples[i].mValue))
		{
			points.push_back(QPoint(dataRect.left() + i, dataRect.height() / 2.0 + dataRect.top() - samples[i].mValue));
		}
	}
//	painter.setRenderHint(QPainter::RenderHint::Antialiasing);
	painter.drawPolyline(points.data(), (int)points.size());
	painter.setPen(chan2Pen);
	painter.drawPoints(points.data(), (int)points.size());

	mHistories.at(1).getSamples(mCurrentLeftTime, mCurrentLeftTime + 10.0 * mTimeDiv, samples.data(), samples.size());
	painter.setPen(chan2Pen);
	points.clear();
	for (int i=0; i<dataRect.width(); ++i)
	{
//		double t = double(i) / dataRect.width() * 2 * M_PI * 7.0 * mTimeDiv;
//		double t = mCurrentLeftTime + double(i) / dataRect.width() * 10.0 * mTimeDiv;
//		points.push_back(QPoint(dataRect.left() + i, dataRect.height() / 2.0 + dataRect.height() * sin(t*1.7) / 3.0 ));
//		points.push_back(QPoint(dataRect.left() + i, samples[i+dataRect.width()].mValue));
		if (!isnan(samples[i].mValue))
		{
			points.push_back(QPoint(dataRect.left() + i, dataRect.height() / 2.0 + dataRect.top() - samples[i].mValue));
		}
	}
//	painter.drawPolyline(points.data() + dataRect.width(), (int)dataRect.width());
	painter.drawPolyline(points.data(), (int)points.size());

	// Measures box
	QRect measRect(surface.width() - 250, surface.height() - 16 * 6 , 240, 16 * 4);
	painter.setBrush(QBrush(QColor(0,0,0,200)));
	painter.setPen(solidPen);
	painter.setRenderHint(QPainter::RenderHint::Antialiasing);
	painter.drawRoundedRect(measRect, 8, 8);
	painter.setRenderHint(QPainter::RenderHint::Antialiasing, false);

	// First channel measures
	painter.setPen(chan1Pen);
	painter.drawText(measRect.left() + 4, measRect.top() + 16,   "Min   12.32RPM Max    426.56RPM");
	painter.setPen(chan2Pen);
	painter.drawText(measRect.left() + 4, measRect.top() + 16*2, "Min      20%   Max        80%");

	// Draw mouse over
	if (mMouseX > dataRect.left() && mMouseX < dataRect.right()
			&& mMouseY > dataRect.top() && mMouseY < dataRect.bottom())
	{
		painter.setPen(QPen(QColor(0,255,255), 2));
		painter.drawLine(mMouseX, 0, mMouseX, dataRect.bottom());

		painter.setPen(QPen(QColor(0,255,255), 1));
		double t = mCurrentLeftTime + double(mMouseX - dataRect.left()) / dataRect.width() * 10.0 * mTimeDiv;
//		double t = double(mMouseX - dataRect.left()) / dataRect.width() * 2 * M_PI * 7.0 * mTimeDiv;
		double y = dataRect.height() / 2.0 + dataRect.height() * sin(t) / 2.0;
		painter.drawLine(0, y, mMouseX, y);


//		t = double(mMouseX - dataRect.left()) / dataRect.width() * 2 * M_PI * 5.0 * mTimeDiv;
		y = dataRect.height() / 2.0 + dataRect.height() * sin(t*1.7) / 3.0;
		painter.drawLine(0, y, mMouseX, y);
	}
}

void Plotter::mouseMoveEvent(QMouseEvent* event)
{
	mMouseX = event->x();
	mMouseY = event->y();
	repaint();
}

void Plotter::leaveEvent(QEvent* event)
{
	mMouseX = -1;
	mMouseY = -1;
	repaint();
}

void Plotter::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Left)
	{
		if (event->modifiers() == Qt::ControlModifier)
		{
			// Zoom in
			mTimeDiv *= 2.0;
			event->accept();
			repaint();
		}
		else if (event->modifiers() == 0)
		{
			mLeftTime -= mTimeDiv * 1.0;
			mLeftTime = std::max(0.0, mLeftTime);
			event->accept();
			repaint();
			mTimer.setInterval(30);
			mTimer.start();
		}
	}
	if (event->key() == Qt::Key_Right)
	{
		if (event->modifiers() == Qt::ControlModifier)
		{
			// Zoom out
			mTimeDiv /= 2.0;
			event->accept();
			repaint();
		}
		else if (event->modifiers() == 0)
		{
			mLeftTime += mTimeDiv * 1.0;
			event->accept();
			repaint();
			mTimer.setInterval(30);
			mTimer.start();
		}
	}
	if (!event->isAccepted())
	{
		parent()->event(event);
	}
}

void Plotter::showEvent(QShowEvent* event)
{
	setFocus();
}

void Plotter::update()
{
	float samples[3];

	static double date = 0.0;

	samples[0] = sin(date*5) * 250 + cos(date * 2) * 50 + cos(date / 2) * 50;
	if (fabs(samples[0]) > 100.0 && samples[0] < 0.0)
		samples[0] += 100.0;
	else if (fabs(samples[0]) > 100.0 && samples[0] > 0.0)
		samples[0] -= 100.0;
	else
		samples[0] = 0.0;
	samples[1] = 50 + sin(date * 2) * 200;
	samples[2] = 1.0 + sin(date / 2) * 0.7;
	addSample(0, date, samples[0]);
	if (int(date * 100) % 10 == 0)
		addSample(1, date, samples[1]);
	addSample(2, date, samples[2]);

	date += 0.03;

	bool final = false;
	if (mCurrentLeftTime != mLeftTime)
	{
		mCurrentLeftTime += (mLeftTime - mCurrentLeftTime) / 3.0;
		if (fabs(mCurrentLeftTime - mLeftTime) < mTimeDiv / 10.0)
		{
			mCurrentLeftTime = mLeftTime;
			final = true;
		}
	}
	repaint();
	if (final)
	{
//		mTimer.stop();
	}
}
