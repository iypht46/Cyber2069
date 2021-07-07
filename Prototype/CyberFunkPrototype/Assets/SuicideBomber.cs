using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class SuicideBomber : MonoBehaviour
{
    enum Stage
    {
        Normal,
        PlayerDetected
    };

    GameObject Player;
    Rigidbody2D rb;
    HP hp;
    Stage CurrentStage;
    float timer;

    Vector3 PlayerLastPosition;
    bool setPosition;

    private UI ui;

    public UnityEvent Explode;
    [SerializeField] private float move_speed;
    [SerializeField] private float dash_speed;
    [SerializeField] private float hit_damage;
    [SerializeField] private float DetectRange;
    [SerializeField] private float DashDelay;

    private void OnEnable()
    {
        CurrentStage = Stage.Normal;
        setPosition = false;
    }

    void Start()
    {
        Player = GameObject.Find("Player").gameObject;
        rb = GetComponent<Rigidbody2D>();
        hp = GetComponent<HP>();
        ui = GameObject.Find("UI").GetComponent<UI>();
        CurrentStage = Stage.Normal;
    }

    void Update()
    {
        if(CurrentStage == Stage.Normal)
        {
            if (!hp.isDead)
            {
                float angle = Mathf.Atan2(Player.transform.position.y - transform.position.y, Player.transform.position.x - transform.position.x);

                float newX = move_speed * Mathf.Cos(angle);
                float newY = move_speed * Mathf.Sin(angle);

                rb.velocity = new Vector2(newX * 1.5f, newY);

                if(DetectRange >= Vector3.Distance(Player.transform.position, transform.position))
                {
                    CurrentStage = Stage.PlayerDetected;
                    timer = DashDelay;
                }
            }
        }
        else if (CurrentStage == Stage.PlayerDetected)
        {
            timer -= Time.deltaTime;

            if (timer <= 0 && !hp.isDead)
            {
                if (!setPosition)
                {
                    PlayerLastPosition = Player.transform.position;

                    float angle = Mathf.Atan2(PlayerLastPosition.y - transform.position.y, PlayerLastPosition.x - transform.position.x);
                    setPosition = true;

                    float newX = dash_speed * Mathf.Cos(angle);
                    float newY = dash_speed * Mathf.Sin(angle);

                    rb.velocity = new Vector2(newX, newY);
                }

                if (Vector3.Distance(PlayerLastPosition, transform.position) < 1)
                {
                    rb.velocity = Vector2.zero;
                    Explode.Invoke();
                    hp.StartCoroutine(hp.Dead(1));
                }

            }
            else
            {
                rb.velocity = Vector2.zero;
            }
        }
    }

    public void Dead()
    {
        ui.score += ui.EnemyScore;
        GameObject.Find("Spawner").GetComponent<SpawnSystem>().GetObjectFromPool("Effect").transform.position = this.transform.position;
        hp.StartCoroutine(hp.Dead(1));
    }

    private void OnTriggerEnter2D(Collider2D other)
    {
        if (!hp.isDead && other.gameObject.tag == "Player" && !other.GetComponent<PlayerController>().GodMode)
        {
            other.GetComponent<HP>().TakeDamage(hit_damage);
            gameObject.SetActive(false);
        }
    }

}
