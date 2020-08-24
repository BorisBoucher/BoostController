#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QTimer>
#include <assert.h>

class Plotter : public QWidget
{
	Q_OBJECT

public:
	struct ChannelInfo
	{
		QString mName;
		QString mUnit;
	};

private:

	class History
	{
	public:

		struct Sample
		{
			float mValue;
			float mMax;
			float mMin;
		};

		~History()
		{
		}

		void init(const ChannelInfo& channelInfo)
		{
			mChannelInfo = channelInfo;
		}

		void addSample(double date, float sample)
		{
			Chunk* chunk;
			if (mChunkMap.empty() || mChunkMap.rbegin()->second.mNbSample == CHUNK_SIZE)
			{
				// need to allocate a new chuk
				auto ib = mChunkMap.insert(std::make_pair(date, Chunk()));
				chunk = &ib.first->second;
				chunk->mBeginDate = chunk->mEndDate = date;

				chunk->mMean = 0.0;
				chunk->mMax = sample;
				chunk->mMin = sample;
			}
			else
			{
				chunk = &mChunkMap.rbegin()->second;
				chunk->mEndDate = date;
			}

			size_t index = chunk->mNbSample;
			chunk->mSamples[index].mDate = date;
			chunk->mSamples[index].mValue = sample;
			chunk->mMax = std::max(chunk->mMax, sample);
			chunk->mMin = std::min(chunk->mMin, sample);
			chunk->mMean += sample;
			chunk->mNbSample++;
		}

		std::pair<double, double> getEra()
		{
			if (mChunkMap.empty())
			{
				return std::make_pair(0.0, 0.0);
			}
			else
			{
				return std::make_pair(mChunkMap.begin()->second.mBeginDate,
									  mChunkMap.rbegin()->second.mEndDate);
			}
		}

		size_t getSamples(double beginDate, double endDate,
						  Sample* outSamples, size_t nbSample)
		{
			if (mChunkMap.empty())
			{
				for (size_t i = 0; i < nbSample; ++i)
				{
					// store one sample in output
					outSamples[i].mValue = NAN;
				}
				return nbSample;
			}
			// get chunk limits
			auto itBegin = mChunkMap.lower_bound(beginDate);
			if (itBegin != mChunkMap.begin())
				--itBegin;
			auto itEnd = mChunkMap.upper_bound(endDate);
			size_t sampleIt = 0;

			double step = (endDate - beginDate) / nbSample;
			double halfStep = step/2.0;
			Sample* endSample = outSamples + nbSample;

			for (double date = beginDate; outSamples < endSample; ++outSamples, date += step)
			{
				// look for the sample matching the date
				if (itBegin != itEnd && sampleIt == itBegin->second.mNbSample)
				{
					// advance to next chunk
					++itBegin;
					sampleIt = 0;
				}
				if (itBegin != mChunkMap.end() && sampleIt < itBegin->second.mNbSample)
				{
					Chunk& chunk = itBegin->second;
					// there is samples available in this chunk
					while (chunk.mSamples[sampleIt].mDate < date - halfStep && sampleIt < chunk.mNbSample)
					{
						// skip sample
						++sampleIt;
					}
					if (chunk.mSamples[sampleIt].mDate >= date - halfStep
							&& chunk.mSamples[sampleIt].mDate < date + halfStep)
					{
						outSamples->mValue = chunk.mSamples[sampleIt].mValue;
					}
					else
					{
						// No data for this date
						outSamples->mValue = NAN;
					}
				}
				else
				{
					// no more samples
					outSamples->mValue = NAN;
				}
			}
			return nbSample;
		}

		const ChannelInfo& getChannelInfos()
		{
			return mChannelInfo;
		}

	private:
		ChannelInfo mChannelInfo;

		// storage for samples :
		//    1 Hour storage @ 10Hz/20 channels :
		//		200 samples/s => 720.000 samples
		//	Store data in chunk of 16 time step.
		//	Each step contains 16 values + min + max + mean for the chunk.
		// Each chunk produce one sample of lesser resolution.
		// When a chunk of any resolution if full, is produce a sample of
		// lower resolution that may create a new chunk.

		static const size_t CHUNK_SIZE = 16;

		struct Chunk
		{
			double mBeginDate;
			double mEndDate;

			int32_t mNbSample;
			struct Value
			{
				double mDate;
				float mValue;
			} mSamples[CHUNK_SIZE];

			float mMin;
			float mMax;
			float mMean;

			Chunk()
			{
				mNbSample = 0;
			}

		};

		// Chunk indexed with chunk's begin date
		std::map<double, Chunk> mChunkMap;
	};
public:
	explicit Plotter(QWidget *parent = nullptr);

	void setChannels(const std::vector<ChannelInfo>& channels)
	{
		for (const auto& ci : channels)
		{
			mHistories.push_back(History());
			mHistories.back().init(ci);
		}
	}

	void addSample(size_t channel, double date, float value)
	{
		assert(channel < mHistories.size());
		mHistories.at(channel).addSample(date, value);
	}

protected:
	int mMouseX = 0;
	int mMouseY = 0;
	double mCurrentLeftTime = 0.0;
	double mLeftTime = 0.0;
	double mTimeDiv = 1.0;
	std::vector<History> mHistories;

	QTimer mTimer {this};

	void paintEvent(QPaintEvent *event) override;

	void mouseMoveEvent(QMouseEvent* event);
	void leaveEvent(QEvent* event);
	void keyPressEvent(QKeyEvent* event);
	void showEvent(QShowEvent* event);

signals:

public slots:
	void update();
};

#endif // PLOTTER_H
