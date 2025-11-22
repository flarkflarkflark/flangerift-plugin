#include "PluginProcessor.h"
#include "PluginEditor.h"

FlangeriftEditor::FlangeriftEditor(FlangeriftProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(600, 400);
}

FlangeriftEditor::~FlangeriftEditor()
{
}

void FlangeriftEditor::paint(juce::Graphics& g)
{
    // Black background
    g.fillAll(juce::Colours::black);
    
    // Orange gradient for title
    juce::ColourGradient gradient(
        juce::Colour(0xfff97316), getWidth() * 0.3f, 60.0f,
        juce::Colour(0xfffb923c), getWidth() * 0.7f, 60.0f,
        false);
    g.setGradientFill(gradient);
    
    // Draw title
    g.setFont(juce::Font(48.0f, juce::Font::bold));
    g.drawText("FLANGERIFT", 0, 40, getWidth(), 80, 
               juce::Justification::centred, true);
    
    // Subtitle
    g.setColour(juce::Colour(0xaafb923c));
    g.setFont(16.0f);
    g.drawText("Interactive Audio Effect Plugin", 0, 100, getWidth(), 30, 
               juce::Justification::centred, true);
    
    // Draw a simple matrix grid representation
    int gridSize = 8;
    int cellSize = 30;
    int gridStartX = (getWidth() - (gridSize * cellSize)) / 2;
    int gridStartY = 160;
    
    for (int row = 0; row < gridSize; ++row)
    {
        for (int col = 0; col < gridSize; ++col)
        {
            juce::Rectangle<int> cell(
                gridStartX + col * cellSize,
                gridStartY + row * cellSize,
                cellSize - 2,
                cellSize - 2
            );
            
            // Darker cells for inactive areas
            if (row < 4 && col < 4)
                g.setColour(juce::Colour(0x33f97316)); // Flanger zone
            else if (row >= 4 && col >= 4)
                g.setColour(juce::Colour(0x33f97316)); // Filter zone
            else
                g.setColour(juce::Colour(0x111a1a1a)); // Inactive zone
            
            g.fillRoundedRectangle(cell.toFloat(), 3.0f);
            
            // Border
            g.setColour(juce::Colour(0x22ea580c));
            g.drawRoundedRectangle(cell.toFloat(), 3.0f, 1.0f);
        }
    }
    
    // Labels
    g.setColour(juce::Colour(0x99fb923c));
    g.setFont(12.0f);
    g.drawText("Flanger Zone", gridStartX, gridStartY + gridSize * cellSize + 10, 
               120, 20, juce::Justification::left, true);
    g.drawText("Filter Zone", gridStartX + (gridSize * cellSize) - 100, 
               gridStartY + gridSize * cellSize + 10, 
               100, 20, juce::Justification::right, true);
}

void FlangeriftEditor::resized()
{
    // Layout components here when you add sliders/buttons
}
