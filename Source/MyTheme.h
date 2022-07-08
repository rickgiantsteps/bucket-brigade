#pragma once
#include <JuceHeader.h>

#define BORDER_WIDTH 1.5f
#define SCALE_TO_KNOB_RATIO 0.85f
#define BRIGHT_COLOUR 0xff2b2d31
#define DARK_COLOUR 0xff0d0d11

class MyLookAndFeel : public LookAndFeel_V4
{
public:
	MyLookAndFeel()
	{
	}

	void drawRotarySlider(Graphics& g,
		int x, int y, int width, int height,
		float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
		Slider& slider) override
	{
		// Useful variables
		const auto radius = jmin(width, height) * 0.5f * SCALE_TO_KNOB_RATIO - BORDER_WIDTH * 0.5f;
		const auto centreX = x + width * 0.5f;
		const auto centreY = y + height * 0.5f;
		const auto rx = centreX - radius;
		const auto ry = centreY - radius;
		const auto rw = radius * 2.0f;

		// Draw knob body
		//g.setColour(Colours::darkblue);
		g.setGradientFill(ColourGradient(brightColour, rx, ry, darkColour.brighter(0.05), rx, ry + rw, false));
		g.fillEllipse(rx, ry, rw, rw);

		// Draw outline
		//g.setColour(Colours::lightblue);
		g.setGradientFill(ColourGradient(brightColour.brighter(0.9), rx, ry, darkColour.darker(0.9), rx, ry + rw, false));
		g.drawEllipse(rx, ry, rw, rw, BORDER_WIDTH);

		// Prepare pointer shape
		Path p;
		const auto pointerLength = radius * 0.33f;
		const auto pointerThickness = 2.0f;
		p.addRectangle(pointerThickness * -0.5f, -radius, pointerThickness, pointerLength);

		// Draw pointer shape
		//const auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
		const auto angle = jmap(sliderPosProportional, rotaryStartAngle, rotaryEndAngle);
		p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
		//g.setColour(Colours::white);
		g.setGradientFill(ColourGradient(Colours::white, rx, ry, Colours::white.darker(0.2f), rx, ry + rw, false));
		g.fillPath(p);

		// Draw ticks
		Path originalTickShape, t;
		auto tickThickness = 2.0f;
		originalTickShape.addEllipse(tickThickness * -0.5f, radius / -SCALE_TO_KNOB_RATIO, tickThickness, tickThickness);
		g.setColour(Colours::black);

		for (int i = 0; i < numTicks; ++i)
		{
			t = originalTickShape;
			auto normValue = pow((float)i / (numTicks - 1), (float)slider.getSkewFactor());
			float angle = jmap(normValue, rotaryStartAngle, rotaryEndAngle);
			t.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
			g.fillPath(t);
		}

	}

	void drawTickBox(Graphics& g, Component& component,
		float x, float y, float w, float h,
		const bool ticked,
		const bool isEnabled,
		const bool shouldDrawButtonAsHighlighted,
		const bool shouldDrawButtonAsDown) override
	{
		ignoreUnused(isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

		Rectangle<float> tickBounds(x, y, w, h);

		g.setColour(Colours::black);
		g.drawRoundedRectangle(tickBounds, 4.0f, 1.6f);

		if (ticked)
		{
			g.setColour(Colours::black);
			auto tick = getTickShape(0.75f);
			g.fillPath(tick, tick.getTransformToScaleToFit(tickBounds.reduced(4, 5).toFloat(), false));
		}
	}

	void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
		float sliderPos,
		float minSliderPos,
		float maxSliderPos,
		const Slider::SliderStyle style, Slider& slider) override
	{
		auto isTwoVal = (style == Slider::SliderStyle::TwoValueVertical || style == Slider::SliderStyle::TwoValueHorizontal);
		auto isThreeVal = (style == Slider::SliderStyle::ThreeValueVertical || style == Slider::SliderStyle::ThreeValueHorizontal);

		auto trackWidth = jmin(6.0f, slider.isHorizontal() ? (float)height * 0.25f : (float)width * 0.25f) + 0.5f;

		Point<float> startPoint(slider.isHorizontal() ? (float)x : (float)x + (float)width * 0.5f,
			slider.isHorizontal() ? (float)y + (float)height * 0.5f : (float)(height + y));

		Point<float> endPoint(slider.isHorizontal() ? (float)(width + x) : startPoint.x,
			slider.isHorizontal() ? startPoint.y : (float)y);

		Path backgroundTrack;
		backgroundTrack.startNewSubPath(startPoint);
		backgroundTrack.lineTo(endPoint);
		ColourGradient darkFiller = ColourGradient(brightColour, 0, height, darkColour, 0, 0, false);
		g.setGradientFill(darkFiller);
		g.strokePath(backgroundTrack, { trackWidth, PathStrokeType::mitered });

		Path valueTrack;
		Point<float> minPoint, maxPoint, thumbPoint;
		auto kx = slider.isHorizontal() ? sliderPos : ((float)x + (float)width * 0.5f);
		auto ky = slider.isHorizontal() ? ((float)y + (float)height * 0.5f) : sliderPos;

		minPoint = startPoint;
		maxPoint = { kx, ky };

		auto thumbWidth = getSliderThumbRadius(slider);

		valueTrack.startNewSubPath(minPoint);
		valueTrack.lineTo(maxPoint);
		auto topColour = Colours::lightgoldenrodyellow;
		ColourGradient filler = ColourGradient(Colours::grey, 0, height, topColour, 0, 0, false);
		g.setGradientFill(filler);
		g.strokePath(valueTrack, { trackWidth, PathStrokeType::mitered});
		ColourGradient thumbFiller = ColourGradient(darkColour, 0, thumbWidth, brightColour, 0, 0, false);
		g.setGradientFill(thumbFiller);
		g.fillRoundedRectangle(Rectangle<float>(static_cast<float> (thumbWidth*2.5f), static_cast<float> (thumbWidth*0.8f)).withCentre(maxPoint), 1.0f);
	}

	void setNumTicks(int newValue)
	{
		numTicks = newValue;
	}

private:
	const Colour brightColour = Colour(BRIGHT_COLOUR);
	const Colour darkColour = Colour(DARK_COLOUR);

	int numTicks = 21;
};