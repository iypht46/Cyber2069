using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PlayerController : MonoBehaviour
{
    enum WeaponType
    {
        Laser,
        MachineGun
    }

    enum DashControlMode
    {
        Cursor,
        Key,
        LROnly
    }

    enum PlayerControlMode
    {
        V1,
        V2,
        V3
    }

    [SerializeField] private WeaponType Weapon = WeaponType.MachineGun;
    [SerializeField] private DashControlMode DashMode = DashControlMode.Key;
    [SerializeField] private PlayerControlMode ControlMode = PlayerControlMode.V1;
    private Vector2 inputDirection = Vector2.zero;
    private Vector2 tempDashDirection = Vector2.zero;

    //machine gun
    [SerializeField] private float firerate = 10;
    [SerializeField] private float bulletSpeed = 10;
    private float timer = 0;

    //Movement 
    [SerializeField] private float move_speed;
    [SerializeField] private float jump_speed;
    [SerializeField] private float dash_speed;
    [SerializeField] private int maxJump;
    [SerializeField] private int maxDash;
    [SerializeField] private int maxDashJump;
    [SerializeField] private float DashTime;
    [SerializeField] private Color OutofDashColor = Color.cyan;

    [SerializeField] public bool GodMode;

    public bool MomemtumOn;

    int remainingJump;
    int remainingDash;
    int remainingDashJump;
    float remainingDashTime;
    float moveVelocity;
    float startGravityScale;

    float angle;

    public bool grounded = true;
    bool jump = false;
    bool dash = false;

    bool AD_input = false;

    private HP hp;
    private Rigidbody2D rb;
    private Collider2D cl;
    private mouseCursor mc;
    [SerializeField] private GameObject Laser;
    private SpawnSystem spawner;
    private TrailRenderer trail;

    private void Start()
    {
        hp = GetComponent<HP>();
        rb = GetComponent<Rigidbody2D>();
        cl = this.GetComponent<Collider2D>();
        mc = GameObject.Find("dot").GetComponent<mouseCursor>();
        spawner = GameObject.Find("Spawner").GetComponent<SpawnSystem>();
        trail = GetComponent<TrailRenderer>();
        //trail.emitting = false;

        startGravityScale = rb.gravityScale;
    }

    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.R))
        {
            Dead();
        }

        if (Input.GetKeyDown(KeyCode.Alpha1))
        {
            Weapon = WeaponType.MachineGun;
            Laser.SetActive(false);
        }
        else if (Input.GetKeyDown(KeyCode.Alpha2))
        {
            Weapon = WeaponType.Laser;
        }

        moveVelocity = 0;

        switch (Weapon)
        {
            case WeaponType.Laser:
                if (Input.GetMouseButton(0))
                {
                    Laser.SetActive(true);
                }
                else
                {
                    Laser.SetActive(false);
                }
                break;
            case WeaponType.MachineGun:
                if (Input.GetMouseButton(0))
                {
                    if (timer <= 0)
                    {
                        GameObject Bullet = spawner.GetObjectFromPool("Bullet");
                        Vector2 cursorPos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
                        Bullet.transform.position = this.transform.position;
                        Bullet.GetComponent<Rigidbody2D>().velocity = (cursorPos - (Vector2)this.transform.position).normalized * bulletSpeed;
                        timer = 1 / firerate;
                    }
                }
                timer -= Time.deltaTime;
                break;
            default:
                break;
        }

        //Left Right Movement
        float x = 0;
        float y = 0;
        if (Input.GetKey(KeyCode.A))
        {
            moveVelocity = -move_speed;
            x -= 1;
        }
        if (Input.GetKey(KeyCode.D))
        {
            moveVelocity = move_speed;
            x += 1;
        }

        if(Input.GetKey(KeyCode.A) || Input.GetKey(KeyCode.D))
        {
            AD_input = true;
        }
        else
        {
            AD_input = false;
        }

        if (Input.GetKey(KeyCode.W))
        {
            y += 1;
        }
        if (Input.GetKey(KeyCode.S))
        {
            y -= 1;
        }
        inputDirection = new Vector2(x, y);

        //through platform
        if (Input.GetKeyDown(KeyCode.S))
        {
            cl.isTrigger = true;
        }
        else if (Input.GetKeyUp(KeyCode.S)) {
            cl.isTrigger = false;
        }
        
        if (ControlMode == PlayerControlMode.V1)
        {
            if (Input.GetKeyDown(KeyCode.Space))
            {
                if (grounded && !jump)
                {
                    rb.velocity = new Vector2(rb.velocity.x, jump_speed);
                    jump = true;
                }
                else if (remainingDash > 0 && !grounded && !dash)
                {
                    jump = false;

                    remainingDash--;
                    remainingDashTime = DashTime;

                    angle = Mathf.Atan2(mc.cursorPos.y - transform.position.y, mc.cursorPos.x - transform.position.x);
                    //if (inputDirection.x == 0)
                    //{
                    //    inputDirection.y = 1;
                    //}
                    tempDashDirection = inputDirection;
                    hp.invincible = true;
                    dash = true;
                    //trail.emitting = true;
                    rb.gravityScale = 0;
                }
            }
        }
        else if (ControlMode == PlayerControlMode.V2)
        {
            if (Input.GetKeyDown(KeyCode.Space))
            {
                if (remainingDashJump > 0)
                {
                    remainingDashJump--;
                    rb.velocity = new Vector2(rb.velocity.x, jump_speed);
                }
            }

            if (Input.GetKeyDown(KeyCode.LeftShift))
            {
                if (remainingDashJump > 0 && !grounded && !dash)
                {

                    remainingDashJump--;
                    remainingDashTime = DashTime;

                    angle = Mathf.Atan2(mc.cursorPos.y - transform.position.y, mc.cursorPos.x - transform.position.x);
                    //if (inputDirection.x == 0)
                    //{
                    //    inputDirection.y = 1;
                    //}
                    tempDashDirection = inputDirection;
                    hp.invincible = true;
                    dash = true;
                    //trail.emitting = true;
                    rb.gravityScale = 0;
                }

            }
        }else if (ControlMode == PlayerControlMode.V3)
        {
            if (Input.GetKeyDown(KeyCode.Space))
            {
                if (remainingJump > 0)
                {
                    remainingJump--;
                    rb.velocity = new Vector2(rb.velocity.x, jump_speed);
                }
            }

            if (Input.GetKeyDown(KeyCode.LeftShift))
            {
                if (remainingDash > 0 && !grounded && !dash)
                {

                    remainingDash--;
                    remainingDashTime = DashTime;

                    angle = Mathf.Atan2(mc.cursorPos.y - transform.position.y, mc.cursorPos.x - transform.position.x);
                    //if (inputDirection.x == 0)
                    //{
                    //    inputDirection.y = 1;
                    //}
                    tempDashDirection = inputDirection;
                    hp.invincible = true;
                    dash = true;
                    //trail.emitting = true;
                    rb.gravityScale = 0;
                }

            }
        }
    }

    void FixedUpdate()
    {
        if (dash)
        {
            Dash();
        }

        if(remainingDash == 0 || remainingJump == 0 || remainingDashJump == 0)
        {
            GetComponent<SpriteRenderer>().color = OutofDashColor;
        }
        else
        {
            GetComponent<SpriteRenderer>().color = Color.white;
        }

        if (MomemtumOn)
        {
            if (grounded || jump || (!dash && AD_input))
            {
                rb.velocity = new Vector2(moveVelocity, rb.velocity.y);
            }
        }
        else
        {
            if (grounded || jump || !dash)
            {
                rb.velocity = new Vector2(moveVelocity, rb.velocity.y);
            }
        }

    }
    //Check if Grounded
    void OnCollisionEnter2D()
    {
        grounded = true;
        jump = false;
        remainingDash = maxDash;
        remainingJump = maxJump;
        remainingDashJump = maxDashJump;

    }
    void OnCollisionExit2D()
    {
        grounded = false;
    }

    void Dash()
    {

        if (remainingDashTime <= 0)
        {
            hp.invincible = false;
            //trail.emitting = false;
            dash = false;
            rb.gravityScale = startGravityScale;
            rb.velocity = new Vector2(rb.velocity.x, rb.velocity.y/2);
        }
        else
        {
            remainingDashTime -= Time.deltaTime;

            switch (DashMode)
            {
                case DashControlMode.Cursor:
                    float newX = dash_speed * Mathf.Cos(angle);
                    float newY = dash_speed * Mathf.Sin(angle);
                    //Debug.Log(newX);
                    //Debug.Log(newY);

                    rb.velocity = new Vector2(newX * 1.5f, newY);
                    break;
                case DashControlMode.Key:
                    if (tempDashDirection != Vector2.zero)
                    {
                        rb.velocity = tempDashDirection.normalized * dash_speed;
                    }
                    break;
                case DashControlMode.LROnly:
                    if (tempDashDirection != Vector2.zero)
                    {
                        tempDashDirection.y = 0;
                        rb.velocity = tempDashDirection.normalized * dash_speed;
                    }
                    break;
            }

        }
    }

    public void Dead()
    {
        SceneManager.LoadScene(SceneManager.GetActiveScene().name);
    }

}
