#include "HighScoreUI.hpp"

void HighScoreUI::OnAwake()
{
	/*nametext = NameText->GetComponent<TextRenderer>();
	scoretext = ScoreText->GetComponent<TextRenderer>();*/
}

void HighScoreUI::OnEnable() 
{
	UpdateScoreBoard();
}

void HighScoreUI::OnDisable()
{
}

void HighScoreUI::OnUpdate(float dt) 
{
}

void HighScoreUI::UpdateScoreBoard() 
{
	ldb = GameController::GetInstance()->GetPlayerData()->Leaderboard;

	s_score = "";
	s_name = "";

	int i = 1;

	for (std::shared_ptr<LeaderboardEntry> en : ldb) 
	{
		//s_highscore += (to_string(i) + "  ");

		s_name += en->Name;
		s_name += "\n";

		s_score += to_string(en->Score);
		s_score += "\n";

		i++;

		if (i > maxEntryShow) 
		{
			break;
		}
	}

	for (auto it = scoreText.begin(); it != scoreText.end(); ++it)
	{
		auto text = (*it)->GetComponent<TextRenderer>();
		if (text)
			text->SetText(s_name);
	}

	for (auto it = nameText.begin(); it != nameText.end(); ++it)
	{
		auto text = (*it)->GetComponent<TextRenderer>();
		if (text)
			text->SetText(s_score);
	}
		
	/*nametext->SetText(s_name);
	scoretext->SetText(s_score);*/
}